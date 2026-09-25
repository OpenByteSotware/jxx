#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslHttpsURLConnection.h"

#include <algorithm>
#include <cctype>
#include <climits>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <openssl/x509.h>
#include <openssl/x509_vfy.h>

#include "ext/net/ssl/jxx.ext.net.ssl.HttpsURLConnection.h"
#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.IOException.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"
#include "net/jxx.net.ProtocolException.h"
#include "net/jxx.net.URL.h"
#include "security/cert/jxx.security.cert.Certificate.h"

namespace jxx::ext::net::ssl::internal {
namespace {

struct BioDeleter { void operator()(BIO* value) const noexcept { if (value) BIO_free_all(value); } };
struct ContextDeleter { void operator()(SSL_CTX* value) const noexcept { if (value) SSL_CTX_free(value); } };
struct X509Deleter { void operator()(X509* value) const noexcept { if (value) X509_free(value); } };

std::string sslError(const char* prefix) {
    const unsigned long code = ERR_get_error();
    if (code == 0UL) return prefix;
    char buffer[256]{};
    ERR_error_string_n(code, buffer, sizeof(buffer));
    return std::string(prefix) + ": " + buffer;
}

std::string lower(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(),
        [](unsigned char c){ return static_cast<char>(std::tolower(c)); });
    return value;
}

class DerCertificate final
    : public ::jxx::lang::ClassBase<DerCertificate, ::jxx::security::cert::Certificate> {
public:
    explicit DerCertificate(const ::jxx::lang::ByteArray& encoded)
        : Super(::jxx::NEW<::jxx::lang::String>("X.509")), encoded_(encoded) {}
    ::jxx::lang::ByteArray getEncoded() const override {
        const auto copy = ::jxx::NEW<::jxx::lang::JxxArray<::jxx::lang::jbyte,1U>>(encoded_->length);
        for (::jxx::lang::jint i=0;i<encoded_->length;++i) (*copy)[i]=(*encoded_)[i];
        return copy;
    }
private:
    ::jxx::lang::ByteArray encoded_;
};

class OpenSslHttpsURLConnection final
    : public ::jxx::lang::ClassBase<OpenSslHttpsURLConnection, ::jxx::ext::net::ssl::HttpsURLConnection> {
public:
    explicit OpenSslHttpsURLConnection(const ::jxx::Ptr<::jxx::net::URL>& url)
        : Super(url) {}

    void connect() override {
        if (connected_) return;
        const auto url = getURL();
        if (url == nullptr || url->getHost() == nullptr) throw ::jxx::lang::IllegalStateException();
        host_ = url->getHost()->utf8();
        const ::jxx::lang::jint port = url->getPort() < 0 ? 443 : url->getPort();
        const std::string endpoint = host_ + ":" + std::to_string(port);

        OPENSSL_init_ssl(OPENSSL_INIT_LOAD_SSL_STRINGS | OPENSSL_INIT_LOAD_CRYPTO_STRINGS, nullptr);
        context_.reset(SSL_CTX_new(TLS_client_method()));
        if (!context_) throw ::jxx::io::IOException(sslError("SSL_CTX_new failed"));
        SSL_CTX_set_min_proto_version(context_.get(), TLS1_2_VERSION);
        SSL_CTX_set_verify(context_.get(), SSL_VERIFY_PEER, nullptr);
        if (SSL_CTX_set_default_verify_paths(context_.get()) != 1)
            throw ::jxx::io::IOException(sslError("could not load default trust paths"));

        connection_.reset(BIO_new_ssl_connect(context_.get()));
        if (!connection_) throw ::jxx::io::IOException(sslError("BIO_new_ssl_connect failed"));
        BIO_set_conn_hostname(connection_.get(), endpoint.c_str());
        SSL* ssl = nullptr;
        BIO_get_ssl(connection_.get(), &ssl);
        if (!ssl) throw ::jxx::io::IOException("TLS session was not created");
        SSL_set_tlsext_host_name(ssl, host_.c_str());
        SSL_set1_host(ssl, host_.c_str());
        if (BIO_do_connect(connection_.get()) <= 0 || BIO_do_handshake(connection_.get()) <= 0)
            throw ::jxx::io::IOException(sslError("TLS connection failed"));
        if (SSL_get_verify_result(ssl) != X509_V_OK)
            throw ::jxx::io::IOException("TLS certificate verification failed");

        cipherSuite_ = ::jxx::NEW<::jxx::lang::String>(SSL_get_cipher_name(ssl));
        capturePeerCertificate(ssl);
        connected_ = true;
    }

    ::jxx::Ptr<::jxx::io::InputStream> getInputStream() override {
        connect();
        if (responseBody_ == nullptr) executeRequest();
        return ::jxx::NEW<::jxx::io::ByteArrayInputStream>(responseBody_);
    }

    ::jxx::lang::jint getResponseCode() const override {
        auto self = const_cast<OpenSslHttpsURLConnection*>(this);
        if (self->responseBody_ == nullptr) self->executeRequest();
        return responseCode_;
    }

    ::jxx::Ptr<::jxx::lang::String> getResponseMessage() const override {
        auto self = const_cast<OpenSslHttpsURLConnection*>(this);
        if (self->responseBody_ == nullptr) self->executeRequest();
        return responseMessage_;
    }

    void disconnect() override { connection_.reset(); context_.reset(); connected_=false; }
    ::jxx::lang::jbool usingProxy() const override { return false; }
    ::jxx::Ptr<::jxx::lang::String> getCipherSuite() const override { const_cast<OpenSslHttpsURLConnection*>(this)->connect(); return cipherSuite_; }
    ::jxx::Ptr<CertificateArray> getLocalCertificates() const override { return nullptr; }
    ::jxx::Ptr<CertificateArray> getServerCertificates() const override { const_cast<OpenSslHttpsURLConnection*>(this)->connect(); return serverCertificates_; }

private:
    void capturePeerCertificate(SSL* ssl) {
        std::unique_ptr<X509,X509Deleter> certificate(SSL_get1_peer_certificate(ssl));
        if (!certificate) return;
        const int length = i2d_X509(certificate.get(), nullptr);
        if (length <= 0) return;
        std::vector<unsigned char> der(static_cast<std::size_t>(length));
        unsigned char* cursor=der.data();
        i2d_X509(certificate.get(), &cursor);
        const auto bytes=::jxx::NEW<::jxx::lang::JxxArray<::jxx::lang::jbyte,1U>>(length);
        for(int i=0;i<length;++i)(*bytes)[i]=static_cast<::jxx::lang::jbyte>(der[static_cast<std::size_t>(i)]);
        serverCertificates_=::jxx::NEW<CertificateArray>(1);
        (*serverCertificates_)[0]=::jxx::NEW<DerCertificate>(bytes);
    }

    void executeRequest() {
        connect();
        const auto url=getURL();
        std::string target=url->getFile()==nullptr?"/":url->getFile()->utf8();
        if(target.empty())target="/";
        const std::string method=getRequestMethod()==nullptr?"GET":getRequestMethod()->utf8();
        if(method!="GET"&&method!="HEAD")throw ::jxx::net::ProtocolException(::jxx::NEW<::jxx::lang::String>("OpenSSL HTTPS backend currently supports GET and HEAD"));
        std::string request=method+" "+target+" HTTP/1.1\r\nHost: "+host_+"\r\nConnection: close\r\nAccept-Encoding: identity\r\n";
        for(const auto& item:requestProps_)request+=item.first+": "+item.second+"\r\n";
        request+="\r\n";
        if(BIO_write(connection_.get(),request.data(),static_cast<int>(request.size()))<=0)throw ::jxx::io::IOException(sslError("HTTPS write failed"));
        std::string response;char buffer[8192];
        for(;;){const int count=BIO_read(connection_.get(),buffer,sizeof(buffer));if(count>0){response.append(buffer,static_cast<std::size_t>(count));continue;}if(count==0)break;if(!BIO_should_retry(connection_.get()))throw ::jxx::io::IOException(sslError("HTTPS read failed"));}
        const auto split=response.find("\r\n\r\n");if(split==std::string::npos)throw ::jxx::io::IOException("Invalid HTTP response");
        std::istringstream headers(response.substr(0,split));std::string status;std::getline(headers,status);if(!status.empty()&&status.back()=='\r')status.pop_back();
        std::istringstream statusLine(status);std::string version;statusLine>>version>>responseCode_;std::string reason;std::getline(statusLine,reason);if(!reason.empty()&&reason.front()==' ')reason.erase(reason.begin());responseMessage_=::jxx::NEW<::jxx::lang::String>(reason);
        std::string line;while(std::getline(headers,line)){if(!line.empty()&&line.back()=='\r')line.pop_back();const auto colon=line.find(':');if(colon!=std::string::npos){auto name=lower(line.substr(0,colon));auto value=line.substr(colon+1);while(!value.empty()&&std::isspace(static_cast<unsigned char>(value.front())))value.erase(value.begin());headerFields_[name]=value;}}
        const std::string body=response.substr(split+4);responseBody_=::jxx::NEW<::jxx::lang::JxxArray<::jxx::lang::jbyte,1U>>(static_cast<::jxx::lang::jint>(body.size()));for(std::size_t i=0;i<body.size();++i)(*responseBody_)[static_cast<::jxx::lang::jint>(i)]=static_cast<::jxx::lang::jbyte>(body[i]);
    }

    std::unique_ptr<SSL_CTX,ContextDeleter> context_;
    std::unique_ptr<BIO,BioDeleter> connection_;
    std::string host_;
    ::jxx::Ptr<::jxx::lang::String> cipherSuite_;
    ::jxx::Ptr<CertificateArray> serverCertificates_;
    ::jxx::lang::ByteArray responseBody_;
};
}

::jxx::Ptr<::jxx::net::URLConnection> openHttpsConnection(const ::jxx::Ptr<::jxx::net::URL>& url) {
    if (url == nullptr) throw ::jxx::lang::NullPointerException();
    return ::jxx::NEW<OpenSslHttpsURLConnection>(url);
}
}
