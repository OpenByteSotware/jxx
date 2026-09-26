#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
namespace jxx::net{class InetSocketAddress;}namespace jxx::com::sun::net::httpserver{class HttpServer;class HttpsServer;}namespace jxx::com::sun::net::httpserver::spi{
class HttpServerProvider:public ::jxx::lang::ClassBase<HttpServerProvider,::jxx::lang::Object>{public:using JxxSuper=::jxx::lang::Object;using Super=::jxx::lang::ClassBase<HttpServerProvider,JxxSuper>;virtual~HttpServerProvider()override=default;static ::jxx::Ptr<HttpServerProvider>provider();virtual ::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpServer>createHttpServer(const ::jxx::Ptr<::jxx::net::InetSocketAddress>&address,::jxx::lang::jint backlog)=0;virtual ::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpsServer>createHttpsServer(const ::jxx::Ptr<::jxx::net::InetSocketAddress>&address,::jxx::lang::jint backlog)=0;protected:HttpServerProvider()=default;};}
