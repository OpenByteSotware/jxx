#pragma once
#include "io/jxx.io.Closeable.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
namespace jxx::io { class InputStream; class OutputStream; }
namespace jxx::net { class InetSocketAddress; class URI; }
namespace jxx::lang { class String; }
namespace jxx::com::sun::net::httpserver {
class Headers; class HttpContext; class HttpPrincipal;
class HttpExchange : public ::jxx::lang::ClassBase<HttpExchange, ::jxx::lang::Object, ::jxx::io::Closeable> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<HttpExchange, JxxSuper, ::jxx::io::Closeable>;
    ~HttpExchange() override = default;
    virtual ::jxx::Ptr<Headers> getRequestHeaders() = 0;
    virtual ::jxx::Ptr<Headers> getResponseHeaders() = 0;
    virtual ::jxx::Ptr<::jxx::net::URI> getRequestURI() = 0;
    virtual ::jxx::Ptr<::jxx::lang::String> getRequestMethod() = 0;
    virtual ::jxx::Ptr<HttpContext> getHttpContext() = 0;
    virtual void close() override = 0;
    virtual ::jxx::Ptr<::jxx::io::InputStream> getRequestBody() = 0;
    virtual ::jxx::Ptr<::jxx::io::OutputStream> getResponseBody() = 0;
    virtual void sendResponseHeaders(::jxx::lang::jint responseCode, ::jxx::lang::jlong responseLength) = 0;
    virtual ::jxx::Ptr<::jxx::net::InetSocketAddress> getRemoteAddress() = 0;
    virtual ::jxx::lang::jint getResponseCode() = 0;
    virtual ::jxx::Ptr<::jxx::net::InetSocketAddress> getLocalAddress() = 0;
    virtual ::jxx::Ptr<::jxx::lang::String> getProtocol() = 0;
    virtual ::jxx::Ptr<::jxx::lang::Object> getAttribute(const ::jxx::Ptr<::jxx::lang::String>& name) = 0;
    virtual void setAttribute(const ::jxx::Ptr<::jxx::lang::String>& name, const ::jxx::Ptr<::jxx::lang::Object>& value) = 0;
    virtual void setStreams(const ::jxx::Ptr<::jxx::io::InputStream>& input, const ::jxx::Ptr<::jxx::io::OutputStream>& output) = 0;
    virtual ::jxx::Ptr<HttpPrincipal> getPrincipal() = 0;
protected:
    HttpExchange() = default;
};
}
