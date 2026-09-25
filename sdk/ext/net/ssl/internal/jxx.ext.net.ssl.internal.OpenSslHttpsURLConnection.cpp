#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslHttpsURLConnection.h"

#include <algorithm>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <openssl/x509.h>
#include <openssl/x509_vfy.h>

#if OPENSSL_VERSION_NUMBER < 0x10101000L
#error "JXX HTTPS requires OpenSSL 1.1.1 or newer for TLS 1.3"
#endif

#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.DerCertificate.h"
#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.IOException.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"
#include "net/jxx.net.ProtocolException.h"
#include "net/jxx.net.URL.h"

namespace jxx::ext::net::ssl::internal {
namespace {

std::string sslError(const char* prefix) {
    const unsigned long code = ERR_get_error();
    if (code == 0UL) {
        return prefix;
    }

    char buffer[256]{};
    ERR_error_string_n(code, buffer, sizeof(buffer));
    return std::string(prefix) + ": " + buffer;
}

std::string lower(std::string value) {
    std::transform(
        value.begin(),
        value.end(),
        value.begin(),
        [](unsigned char current) {
            return static_cast<char>(std::tolower(current));
        });
    return value;
}

} // namespace

OpenSslHttpsURLConnection::OpenSslHttpsURLConnection(
    const ::jxx::Ptr<::jxx::net::URL>& url)
    : Super(url) {
}

OpenSslHttpsURLConnection::~OpenSslHttpsURLConnection() {
    releaseNativeResources();
}

void OpenSslHttpsURLConnection::releaseNativeResources() noexcept {
    if (connection_ != nullptr) {
        BIO_free_all(connection_);
        connection_ = nullptr;
    }
    if (context_ != nullptr) {
        SSL_CTX_free(context_);
        context_ = nullptr;
    }
}

void OpenSslHttpsURLConnection::connect() {
    if (connected_) {
        return;
    }

    const auto url = getURL();
    if (url == nullptr || url->getHost() == nullptr) {
        throw ::jxx::lang::IllegalStateException();
    }

    host_ = url->getHost()->utf8();
    const ::jxx::lang::jint port =
        url->getPort() < 0 ? 443 : url->getPort();
    const std::string endpoint =
        host_ + ":" + std::to_string(port);

    OPENSSL_init_ssl(
        OPENSSL_INIT_LOAD_SSL_STRINGS |
            OPENSSL_INIT_LOAD_CRYPTO_STRINGS,
        nullptr);

    context_ = SSL_CTX_new(TLS_client_method());
    if (context_ == nullptr) {
        throw ::jxx::io::IOException(
            sslError("SSL_CTX_new failed"));
    }

    try {
        if (SSL_CTX_set_min_proto_version(
                context_, TLS1_2_VERSION) != 1) {
            throw ::jxx::io::IOException(
                sslError("could not set TLS 1.2 minimum"));
        }
        if (SSL_CTX_set_max_proto_version(
                context_, TLS1_3_VERSION) != 1) {
            throw ::jxx::io::IOException(
                sslError("could not set TLS 1.3 maximum"));
        }

        SSL_CTX_set_verify(context_, SSL_VERIFY_PEER, nullptr);
        if (SSL_CTX_set_default_verify_paths(context_) != 1) {
            throw ::jxx::io::IOException(
                sslError("could not load default trust paths"));
        }

        connection_ = BIO_new_ssl_connect(context_);
        if (connection_ == nullptr) {
            throw ::jxx::io::IOException(
                sslError("BIO_new_ssl_connect failed"));
        }

        BIO_set_conn_hostname(connection_, endpoint.c_str());

        SSL* ssl = nullptr;
        BIO_get_ssl(connection_, &ssl);
        if (ssl == nullptr) {
            throw ::jxx::io::IOException(
                "TLS session was not created");
        }

        if (SSL_set_tlsext_host_name(ssl, host_.c_str()) != 1) {
            throw ::jxx::io::IOException(
                sslError("could not set TLS SNI host"));
        }
        if (SSL_set1_host(ssl, host_.c_str()) != 1) {
            throw ::jxx::io::IOException(
                sslError("could not set TLS verification host"));
        }

        if (BIO_do_connect(connection_) <= 0 ||
            BIO_do_handshake(connection_) <= 0) {
            throw ::jxx::io::IOException(
                sslError("TLS connection failed"));
        }
        if (SSL_get_verify_result(ssl) != X509_V_OK) {
            throw ::jxx::io::IOException(
                "TLS certificate verification failed");
        }

        cipherSuite_ = ::jxx::NEW<::jxx::lang::String>(
            SSL_get_cipher_name(ssl));
        capturePeerCertificate(ssl);
        connected_ = true;
    } catch (...) {
        releaseNativeResources();
        throw;
    }
}

::jxx::Ptr<::jxx::io::InputStream>
OpenSslHttpsURLConnection::getInputStream() {
    connect();
    if (responseBody_ == nullptr) {
        executeRequest();
    }
    return ::jxx::NEW<::jxx::io::ByteArrayInputStream>(
        responseBody_);
}

::jxx::lang::jint
OpenSslHttpsURLConnection::getResponseCode() const {
    auto* self = const_cast<OpenSslHttpsURLConnection*>(this);
    if (self->responseBody_ == nullptr) {
        self->executeRequest();
    }
    return responseCode_;
}

::jxx::Ptr<::jxx::lang::String>
OpenSslHttpsURLConnection::getResponseMessage() const {
    auto* self = const_cast<OpenSslHttpsURLConnection*>(this);
    if (self->responseBody_ == nullptr) {
        self->executeRequest();
    }
    return responseMessage_;
}

void OpenSslHttpsURLConnection::disconnect() {
    releaseNativeResources();
    connected_ = false;
}

::jxx::lang::jbool
OpenSslHttpsURLConnection::usingProxy() const {
    return false;
}

::jxx::Ptr<::jxx::lang::String>
OpenSslHttpsURLConnection::getCipherSuite() const {
    const_cast<OpenSslHttpsURLConnection*>(this)->connect();
    return cipherSuite_;
}

::jxx::Ptr<OpenSslHttpsURLConnection::CertificateArray>
OpenSslHttpsURLConnection::getLocalCertificates() const {
    return nullptr;
}

::jxx::Ptr<OpenSslHttpsURLConnection::CertificateArray>
OpenSslHttpsURLConnection::getServerCertificates() const {
    const_cast<OpenSslHttpsURLConnection*>(this)->connect();
    return serverCertificates_;
}

void OpenSslHttpsURLConnection::capturePeerCertificate(SSL* ssl) {
    // OpenSSL 1.1.1w returns an incremented certificate reference.
    // The reference is released below with X509_free().
    X509* certificate = SSL_get_peer_certificate(ssl);
    if (certificate == nullptr) {
        return;
    }

    const int length = i2d_X509(certificate, nullptr);
    if (length <= 0) {
        X509_free(certificate);
        return;
    }

    std::vector<unsigned char> der(
        static_cast<std::size_t>(length));
    unsigned char* cursor = der.data();
    i2d_X509(certificate, &cursor);
    X509_free(certificate);

    const auto bytes =
        ::jxx::NEW<
            ::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>(
                length);
    for (int index = 0; index < length; ++index) {
        (*bytes)[index] = static_cast<::jxx::lang::jbyte>(
            der[static_cast<std::size_t>(index)]);
    }

    serverCertificates_ = ::jxx::NEW<CertificateArray>(1);
    (*serverCertificates_)[0] =
        ::jxx::NEW<DerCertificate>(bytes);
}

void OpenSslHttpsURLConnection::executeRequest() {
    connect();

    const auto url = getURL();
    std::string target =
        url->getFile() == nullptr
            ? "/"
            : url->getFile()->utf8();
    if (target.empty()) {
        target = "/";
    }

    const std::string method =
        getRequestMethod() == nullptr
            ? "GET"
            : getRequestMethod()->utf8();
    if (method != "GET" && method != "HEAD") {
        throw ::jxx::net::ProtocolException(
            ::jxx::NEW<::jxx::lang::String>(
                "OpenSSL HTTPS backend currently supports GET and HEAD"));
    }

    std::string request =
        method + " " + target + " HTTP/1.1\r\n" +
        "Host: " + host_ + "\r\n" +
        "Connection: close\r\n" +
        "Accept-Encoding: identity\r\n";
    for (const auto& item : requestProps_) {
        request += item.first + ": " + item.second + "\r\n";
    }
    request += "\r\n";

    if (BIO_write(
            connection_,
            request.data(),
            static_cast<int>(request.size())) <= 0) {
        throw ::jxx::io::IOException(
            sslError("HTTPS write failed"));
    }

    std::string response;
    char buffer[8192];
    for (;;) {
        const int count = BIO_read(
            connection_, buffer, sizeof(buffer));
        if (count > 0) {
            response.append(
                buffer, static_cast<std::size_t>(count));
            continue;
        }
        if (count == 0) {
            break;
        }
        if (!BIO_should_retry(connection_)) {
            throw ::jxx::io::IOException(
                sslError("HTTPS read failed"));
        }
    }

    const auto split = response.find("\r\n\r\n");
    if (split == std::string::npos) {
        throw ::jxx::io::IOException("Invalid HTTP response");
    }

    std::istringstream headers(response.substr(0, split));
    std::string status;
    std::getline(headers, status);
    if (!status.empty() && status.back() == '\r') {
        status.pop_back();
    }

    std::istringstream statusLine(status);
    std::string version;
    statusLine >> version >> responseCode_;
    std::string reason;
    std::getline(statusLine, reason);
    if (!reason.empty() && reason.front() == ' ') {
        reason.erase(reason.begin());
    }
    responseMessage_ =
        ::jxx::NEW<::jxx::lang::String>(reason);

    std::string line;
    while (std::getline(headers, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        const auto colon = line.find(':');
        if (colon == std::string::npos) {
            continue;
        }
        auto name = lower(line.substr(0, colon));
        auto value = line.substr(colon + 1);
        while (!value.empty() &&
               std::isspace(
                   static_cast<unsigned char>(value.front()))) {
            value.erase(value.begin());
        }
        headerFields_[name] = value;
    }

    const std::string body = response.substr(split + 4);
    responseBody_ =
        ::jxx::NEW<
            ::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>(
                static_cast<::jxx::lang::jint>(body.size()));
    for (std::size_t index = 0; index < body.size(); ++index) {
        (*responseBody_)[static_cast<::jxx::lang::jint>(index)] =
            static_cast<::jxx::lang::jbyte>(body[index]);
    }
}

::jxx::Ptr<::jxx::net::URLConnection>
openHttpsConnection(
    const ::jxx::Ptr<::jxx::net::URL>& url) {
    if (url == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    return ::jxx::NEW<OpenSslHttpsURLConnection>(url);
}

} // namespace jxx::ext::net::ssl::internal
