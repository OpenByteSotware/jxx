#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.DefaultHttpServerProvider.h"
#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.DefaultHttpServer.h"
#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.DefaultHttpsServer.h"
namespace jxx::com::sun::net::httpserver::internal
{
	::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpServer> DefaultHttpServerProvider::createHttpServer(const ::jxx::Ptr<::jxx::net::InetSocketAddress>& a, ::jxx::lang::jint b)
	{
		return a ? ::jxx::CAST<::jxx::com::sun::net::httpserver::HttpServer>(::jxx::NEW<DefaultHttpServer>(a, b)) : ::jxx::CAST<::jxx::com::sun::net::httpserver::HttpServer>(::jxx::NEW<DefaultHttpServer>());
	}
	::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpsServer> DefaultHttpServerProvider::createHttpsServer(const ::jxx::Ptr<::jxx::net::InetSocketAddress>& a, ::jxx::lang::jint b)
	{
		return a ? ::jxx::CAST<::jxx::com::sun::net::httpserver::HttpsServer>(::jxx::NEW<DefaultHttpsServer>(a, b)): :: jxx::CAST<::jxx::com::sun::net::httpserver::HttpsServer>(::jxx::NEW<DefaultHttpsServer>());
	}
}
