#pragma once
#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.HttpServer.h"
namespace jxx::com::sun::net::httpserver { class HttpsConfigurator;
class HttpsServer : public ::jxx::lang::ClassBase<HttpsServer,HttpServer> {
public: using JxxSuper=HttpServer; using Super=::jxx::lang::ClassBase<HttpsServer,JxxSuper>; virtual ~HttpsServer()override=default;
 static ::jxx::Ptr<HttpsServer> create(); static ::jxx::Ptr<HttpsServer> create(const ::jxx::Ptr<::jxx::net::InetSocketAddress>& address,::jxx::lang::jint backlog);
 virtual void setHttpsConfigurator(const ::jxx::Ptr<HttpsConfigurator>& configurator)=0; virtual ::jxx::Ptr<HttpsConfigurator> getHttpsConfigurator()=0;
protected: HttpsServer()=default;
}; }
