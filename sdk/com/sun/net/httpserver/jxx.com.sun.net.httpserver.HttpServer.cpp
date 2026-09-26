#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.HttpServer.h"
#include "com/sun/net/httpserver/spi/jxx.com.sun.net.httpserver.spi.HttpServerProvider.h"

namespace jxx::com::sun::net::httpserver {

::jxx::Ptr<HttpServer> HttpServer::create()
{
    return spi::HttpServerProvider::provider()
        ->createHttpServer(nullptr, 0);
}

::jxx::Ptr<HttpServer> HttpServer::create(
    const ::jxx::Ptr<::jxx::net::InetSocketAddress>& address,
    ::jxx::lang::jint backlog)
{
    return spi::HttpServerProvider::provider()
        ->createHttpServer(address, backlog);
}

} // namespace jxx::com::sun::net::httpserver
