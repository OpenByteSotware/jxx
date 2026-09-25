#pragma once

#include <string>

#include "ext/net/ssl/jxx.ext.net.ssl.HttpsURLConnection.h"
#include "lang/jxx.lang.buildin_array.h"

// OpenSSL native types are forward declared to keep OpenSSL headers out of
// the JXX class header while preserving a complete class declaration here.
typedef struct ssl_ctx_st SSL_CTX;
typedef struct bio_st BIO;
typedef struct ssl_st SSL;

namespace jxx::io {
class InputStream;
}

namespace jxx::net {
class URL;
class URLConnection;
}

namespace jxx::ext::net::ssl::internal {

class OpenSslHttpsURLConnection final
    : public ::jxx::lang::ClassBase<
          OpenSslHttpsURLConnection,
          ::jxx::ext::net::ssl::HttpsURLConnection> {
public:
    using JxxSuper = ::jxx::ext::net::ssl::HttpsURLConnection;
    using Super =
        ::jxx::lang::ClassBase<OpenSslHttpsURLConnection, JxxSuper>;
    using CertificateArray = JxxSuper::CertificateArray;

    explicit OpenSslHttpsURLConnection(
        const ::jxx::Ptr<::jxx::net::URL>& url);

    ~OpenSslHttpsURLConnection() override;

    void connect() override;

    ::jxx::Ptr<::jxx::io::InputStream>
    getInputStream() override;

    ::jxx::lang::jint
    getResponseCode() const override;

    ::jxx::Ptr<::jxx::lang::String>
    getResponseMessage() const override;

    void disconnect() override;

    ::jxx::lang::jbool
    usingProxy() const override;

    ::jxx::Ptr<::jxx::lang::String>
    getCipherSuite() const override;

    ::jxx::Ptr<CertificateArray>
    getLocalCertificates() const override;

    ::jxx::Ptr<CertificateArray>
    getServerCertificates() const override;

private:
    void capturePeerCertificate(SSL* ssl);
    void executeRequest();
    void releaseNativeResources() noexcept;

    SSL_CTX* context_ = nullptr;
    BIO* connection_ = nullptr;
    std::string host_;
    ::jxx::Ptr<::jxx::lang::String> cipherSuite_;
    ::jxx::Ptr<CertificateArray> serverCertificates_;
    ::jxx::lang::ByteArray responseBody_;
};

::jxx::Ptr<::jxx::net::URLConnection>
openHttpsConnection(
    const ::jxx::Ptr<::jxx::net::URL>& url);

} // namespace jxx::ext::net::ssl::internal
