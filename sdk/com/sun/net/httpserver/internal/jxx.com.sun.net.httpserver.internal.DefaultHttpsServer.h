#pragma once
#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.HttpsServer.h"
#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.DefaultHttpServer.h"
namespace jxx::com::sun::net::httpserver::internal {
class DefaultHttpsServer final : public ::jxx::lang::ClassBase<DefaultHttpsServer, ::jxx::com::sun::net::httpserver::HttpsServer> {
public:
    using JxxSuper=::jxx::com::sun::net::httpserver::HttpsServer; using Super=::jxx::lang::ClassBase<DefaultHttpsServer,JxxSuper>;
    DefaultHttpsServer(); DefaultHttpsServer(const ::jxx::Ptr<::jxx::net::InetSocketAddress>& address,::jxx::lang::jint backlog); ~DefaultHttpsServer() override=default;
    void setHttpsConfigurator(const ::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpsConfigurator>& config) override; ::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpsConfigurator> getHttpsConfigurator() override;
    void bind(const ::jxx::Ptr<::jxx::net::InetSocketAddress>& address,::jxx::lang::jint backlog) override; void start() override; void stop(::jxx::lang::jint delay) override;
    void setExecutor(const ::jxx::Ptr<::jxx::util::concurrent::Executor>& executor) override; ::jxx::Ptr<::jxx::util::concurrent::Executor> getExecutor() override;
    ::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpContext> createContext(const ::jxx::Ptr<::jxx::lang::String>& path,const ::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpHandler>& handler) override; ::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpContext> createContext(const ::jxx::Ptr<::jxx::lang::String>& path) override;
    void removeContext(const ::jxx::Ptr<::jxx::lang::String>& path) override; void removeContext(const ::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpContext>& context) override; ::jxx::Ptr<::jxx::net::InetSocketAddress> getAddress() override;
private:
    ::jxx::Ptr<DefaultHttpServer> delegate_; ::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpsConfigurator> configurator_;
}; }
