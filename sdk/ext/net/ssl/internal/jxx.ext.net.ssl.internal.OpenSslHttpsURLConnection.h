#pragma once

#include <string>

#include "ext/net/ssl/jxx.ext.net.ssl.HttpsURLConnection.h"

namespace jxx::io {
class InputStream;
class OutputStream;
}

namespace jxx::net {
class URL;
class URLConnection;
}

namespace jxx::ext::net::ssl {
class SSLSession;
class SSLSocket;
}

namespace jxx::ext::net::ssl::internal {

class OpenSslHttpsURLConnection final
    : public ::jxx::lang::ClassBase<
          OpenSslHttpsURLConnection,
          ::jxx::ext::net::ssl::HttpsURLConnection> {
public:
    using JxxSuper = ::jxx::ext::net::ssl::HttpsURLConnection;
    using Super = ::jxx::lang::ClassBase<
        OpenSslHttpsURLConnection,
        JxxSuper>;
    using CertificateArray = JxxSuper::CertificateArray;

    explicit OpenSslHttpsURLConnection(
        const ::jxx::Ptr<::jxx::net::URL>& url);

    ~OpenSslHttpsURLConnection() override;

    void connect() override;

    ::jxx::Ptr<::jxx::io::InputStream>
    getInputStream() override;

    ::jxx::lang::jint
    getResponseCode() override;

    ::jxx::Ptr<::jxx::lang::String>
    getResponseMessage() override;

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
    void captureSessionCertificates();
    void executeRequest();
    void releaseSocket() noexcept;

    std::string host_;
    ::jxx::Ptr<::jxx::ext::net::ssl::SSLSocket> socket_;
    ::jxx::Ptr<::jxx::ext::net::ssl::SSLSession> session_;
    ::jxx::Ptr<::jxx::io::InputStream> input_;
    ::jxx::Ptr<::jxx::io::OutputStream> output_;
    ::jxx::Ptr<::jxx::lang::String> cipherSuite_;
    ::jxx::Ptr<CertificateArray> serverCertificates_;
    ::jxx::Ptr<CertificateArray> localCertificates_;
    ::jxx::lang::ByteArray responseBody_;
};

::jxx::Ptr<::jxx::net::URLConnection>
openHttpsConnection(
    const ::jxx::Ptr<::jxx::net::URL>& url);

} // namespace jxx::ext::net::ssl::internal
