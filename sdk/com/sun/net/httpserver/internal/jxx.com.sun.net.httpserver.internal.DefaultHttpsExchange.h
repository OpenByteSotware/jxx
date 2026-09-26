#pragma once

#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.HttpsExchange.h"

namespace jxx::ext::net::ssl {
class SSLSession;
}

namespace jxx::com::sun::net::httpserver::internal {

class DefaultHttpExchange;

class DefaultHttpsExchange final
    : public ::jxx::lang::ClassBase<
          DefaultHttpsExchange,
          ::jxx::com::sun::net::httpserver::HttpsExchange> {
public:
    using JxxSuper =
        ::jxx::com::sun::net::httpserver::HttpsExchange;
    using Super =
        ::jxx::lang::ClassBase<DefaultHttpsExchange, JxxSuper>;

    DefaultHttpsExchange(
        const ::jxx::Ptr<DefaultHttpExchange>& delegate,
        const ::jxx::Ptr<::jxx::ext::net::ssl::SSLSession>& session);

    ~DefaultHttpsExchange() override = default;

    ::jxx::Ptr<::jxx::ext::net::ssl::SSLSession>
    getSSLSession() override;

    ::jxx::Ptr<::jxx::com::sun::net::httpserver::Headers>
    getRequestHeaders() override;
    ::jxx::Ptr<::jxx::com::sun::net::httpserver::Headers>
    getResponseHeaders() override;
    ::jxx::Ptr<::jxx::net::URI> getRequestURI() override;
    ::jxx::Ptr<::jxx::lang::String> getRequestMethod() override;
    ::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpContext>
    getHttpContext() override;
    void close() override;
    ::jxx::Ptr<::jxx::io::InputStream> getRequestBody() override;
    ::jxx::Ptr<::jxx::io::OutputStream> getResponseBody() override;
    void sendResponseHeaders(
        ::jxx::lang::jint responseCode,
        ::jxx::lang::jlong responseLength) override;
    ::jxx::Ptr<::jxx::net::InetSocketAddress>
    getRemoteAddress() override;
    ::jxx::lang::jint getResponseCode() override;
    ::jxx::Ptr<::jxx::net::InetSocketAddress>
    getLocalAddress() override;
    ::jxx::Ptr<::jxx::lang::String> getProtocol() override;
    ::jxx::Ptr<::jxx::lang::Object> getAttribute(
        const ::jxx::Ptr<::jxx::lang::String>& name) override;
    void setAttribute(
        const ::jxx::Ptr<::jxx::lang::String>& name,
        const ::jxx::Ptr<::jxx::lang::Object>& value) override;
    void setStreams(
        const ::jxx::Ptr<::jxx::io::InputStream>& input,
        const ::jxx::Ptr<::jxx::io::OutputStream>& output) override;
    ::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpPrincipal>
    getPrincipal() override;

private:
    ::jxx::Ptr<DefaultHttpExchange> delegate_;
    ::jxx::Ptr<::jxx::ext::net::ssl::SSLSession> session_;
};

} // namespace jxx::com::sun::net::httpserver::internal
