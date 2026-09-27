#pragma once
#include <atomic><mutex><thread><vector>
#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.HttpServer.h"
#include "net/jxx.net.ServerSocket.h"
namespace jxx::com::sun::net::httpserver { class HttpsConfigurator; }
namespace jxx::com::sun::net::httpserver::internal {
class DefaultHttpServer final : public ::jxx::lang::ClassBase<DefaultHttpServer,::jxx::com::sun::net::httpserver::HttpServer> {
public:
 using JxxSuper=::jxx::com::sun::net::httpserver::HttpServer; using Super=::jxx::lang::ClassBase<DefaultHttpServer,JxxSuper>;
 DefaultHttpServer(); DefaultHttpServer(const ::jxx::Ptr<::jxx::net::InetSocketAddress>&addr,::jxx::lang::jint backlog); DefaultHttpServer(const ::jxx::Ptr<::jxx::net::ServerSocket>& listener,const ::jxx::Ptr<::jxx::net::InetSocketAddress>& address,const ::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpsConfigurator>& configurator);~DefaultHttpServer()override;
 void bind(const ::jxx::Ptr<::jxx::net::InetSocketAddress>&addr,::jxx::lang::jint backlog)override;void start()override;void stop(::jxx::lang::jint delay)override;void setExecutor(const ::jxx::Ptr<::jxx::util::concurrent::Executor>&executor)override;::jxx::Ptr<::jxx::util::concurrent::Executor>getExecutor()override;
 ::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpContext>createContext(const ::jxx::Ptr<::jxx::lang::String>&path,const ::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpHandler>&handler)override;::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpContext>createContext(const ::jxx::Ptr<::jxx::lang::String>&path)override;void removeContext(const ::jxx::Ptr<::jxx::lang::String>&path)override;void removeContext(const ::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpContext>&context)override;::jxx::Ptr<::jxx::net::InetSocketAddress>getAddress()override;
private:void acceptLoop();void serve(const ::jxx::Ptr<::jxx::net::Socket>&socket);::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpContext>match(const std::string&path);
 ::jxx::Ptr<::jxx::net::ServerSocket>serverSocket_;::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpsConfigurator>httpsConfigurator_;::jxx::Ptr<::jxx::net::InetSocketAddress>address_;::jxx::Ptr<::jxx::util::concurrent::Executor>executor_;std::mutex mutex_;std::vector<::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpContext>>contexts_;std::atomic<bool>running_{false};std::thread acceptThread_;std::vector<std::thread>workers_;
}; }
