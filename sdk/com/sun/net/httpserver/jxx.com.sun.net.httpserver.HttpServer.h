#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
namespace jxx::lang { class String; }
namespace jxx::net { class InetSocketAddress; }
namespace jxx::util::concurrent { class Executor; }
namespace jxx::com::sun::net::httpserver {
class HttpContext; class HttpHandler;
class HttpServer : public ::jxx::lang::ClassBase<HttpServer, ::jxx::lang::Object> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<HttpServer, JxxSuper>;
    ~HttpServer() override = default;
    static ::jxx::Ptr<HttpServer> create();
    static ::jxx::Ptr<HttpServer> create(const ::jxx::Ptr<::jxx::net::InetSocketAddress>& addr, ::jxx::lang::jint backlog);
    virtual void bind(const ::jxx::Ptr<::jxx::net::InetSocketAddress>& addr, ::jxx::lang::jint backlog) = 0;
    virtual void start() = 0;
    virtual void stop(::jxx::lang::jint delay) = 0;
    virtual void setExecutor(const ::jxx::Ptr<::jxx::util::concurrent::Executor>& executor) = 0;
    virtual ::jxx::Ptr<::jxx::util::concurrent::Executor> getExecutor() = 0;
    virtual ::jxx::Ptr<HttpContext> createContext(const ::jxx::Ptr<::jxx::lang::String>& path, const ::jxx::Ptr<HttpHandler>& handler) = 0;
    virtual ::jxx::Ptr<HttpContext> createContext(const ::jxx::Ptr<::jxx::lang::String>& path) = 0;
    virtual void removeContext(const ::jxx::Ptr<::jxx::lang::String>& path) = 0;
    virtual void removeContext(const ::jxx::Ptr<HttpContext>& context) = 0;
    virtual ::jxx::Ptr<::jxx::net::InetSocketAddress> getAddress() = 0;
protected:
    HttpServer() = default;
};
}
