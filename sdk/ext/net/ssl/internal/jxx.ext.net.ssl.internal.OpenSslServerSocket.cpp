#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslServerSocket.h"

#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslContextConfig.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslSocket.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "io/jxx.io.IOException.h"
#include <openssl/ssl.h>

namespace jxx::ext::net::ssl::internal {
namespace {
::jxx::Ptr<OpenSslServerSocket::StringArray> toArray(const std::vector<std::string>& values){auto r=::jxx::NEW<OpenSslServerSocket::StringArray>(static_cast<::jxx::lang::jint>(values.size()));for(std::size_t i=0;i<values.size();++i)(*r)[static_cast<::jxx::lang::jint>(i)]=::jxx::NEW<::jxx::lang::String>(values[i]);return r;}
std::vector<std::string> toVector(const ::jxx::Ptr<OpenSslServerSocket::StringArray>& values){if(values==nullptr)throw ::jxx::lang::IllegalArgumentException();std::vector<std::string> r;for(::jxx::lang::jint i=0;i<values->length;++i){if((*values)[i]==nullptr)throw ::jxx::lang::IllegalArgumentException();r.push_back((*values)[i]->utf8());}return r;}
}
OpenSslServerSocket::OpenSslServerSocket(
    ::jxx::lang::jint port, ::jxx::lang::jint backlog,
    const ::jxx::Ptr<::jxx::net::InetAddress>& address,
    const std::shared_ptr<OpenSslContextConfig>& config)
    : Super(port, backlog, address), config_(config),
      enabledProtocols_({"TLSv1.2", "TLSv1.3"}) {
    const auto suites = getSupportedCipherSuites();
    for (::jxx::lang::jint index = 0; index < suites->length; ++index)
        enabledCipherSuites_.push_back((*suites)[index]->utf8());
}
::jxx::Ptr<::jxx::net::Socket> OpenSslServerSocket::accept(){auto transport=::jxx::net::ServerSocket::accept();auto socket=::jxx::NEW<OpenSslSocket>(transport,::jxx::NEW<::jxx::lang::String>(""),transport->getPort(),true,std::vector<unsigned char>(),config_);socket->setUseClientMode(false);socket->setNeedClientAuth(needClientAuth_);socket->setWantClientAuth(wantClientAuth_);socket->setEnableSessionCreation(enableSessionCreation_);if(!enabledProtocols_.empty())socket->setEnabledProtocols(toArray(enabledProtocols_));if(!enabledCipherSuites_.empty())socket->setEnabledCipherSuites(toArray(enabledCipherSuites_));return socket;}
::jxx::Ptr<OpenSslServerSocket::StringArray> OpenSslServerSocket::getEnabledCipherSuites()const{return toArray(enabledCipherSuites_);}void OpenSslServerSocket::setEnabledCipherSuites(const ::jxx::Ptr<StringArray>&v){enabledCipherSuites_=toVector(v);}::jxx::Ptr<OpenSslServerSocket::StringArray>
OpenSslServerSocket::getSupportedCipherSuites() const {
    SSL_CTX* context = SSL_CTX_new(TLS_server_method());
    if (context == nullptr) throw ::jxx::io::IOException("SSL_CTX_new failed");
    STACK_OF(SSL_CIPHER)* ciphers = SSL_CTX_get_ciphers(context);
    const int count = ciphers == nullptr ? 0 : sk_SSL_CIPHER_num(ciphers);
    const auto result = ::jxx::NEW<StringArray>(count);
    for (int index = 0; index < count; ++index) {
        const SSL_CIPHER* cipher = sk_SSL_CIPHER_value(ciphers, index);
        const char* name = cipher == nullptr ? nullptr : SSL_CIPHER_get_name(cipher);
        (*result)[index] = ::jxx::NEW<::jxx::lang::String>(name == nullptr ? "" : name);
    }
    SSL_CTX_free(context);
    return result;
}::jxx::Ptr<OpenSslServerSocket::StringArray> OpenSslServerSocket::getEnabledProtocols()const{return toArray(enabledProtocols_);}void OpenSslServerSocket::setEnabledProtocols(const ::jxx::Ptr<StringArray>&v){enabledProtocols_=toVector(v);}::jxx::Ptr<OpenSslServerSocket::StringArray> OpenSslServerSocket::getSupportedProtocols()const{return toArray({"TLSv1.2","TLSv1.3"});}
void OpenSslServerSocket::setNeedClientAuth(::jxx::lang::jbool v){needClientAuth_=v;if(v)wantClientAuth_=false;}::jxx::lang::jbool OpenSslServerSocket::getNeedClientAuth()const{return needClientAuth_;}void OpenSslServerSocket::setWantClientAuth(::jxx::lang::jbool v){wantClientAuth_=v;if(v)needClientAuth_=false;}::jxx::lang::jbool OpenSslServerSocket::getWantClientAuth()const{return wantClientAuth_;}void OpenSslServerSocket::setUseClientMode(::jxx::lang::jbool v){useClientMode_=v;}::jxx::lang::jbool OpenSslServerSocket::getUseClientMode()const{return useClientMode_;}void OpenSslServerSocket::setEnableSessionCreation(::jxx::lang::jbool v){enableSessionCreation_=v;}::jxx::lang::jbool OpenSslServerSocket::getEnableSessionCreation()const{return enableSessionCreation_;}
}
