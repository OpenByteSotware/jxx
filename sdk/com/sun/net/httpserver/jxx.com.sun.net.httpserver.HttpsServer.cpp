#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.HttpsServer.h"
#include "com/sun/net/httpserver/spi/jxx.com.sun.net.httpserver.spi.HttpServerProvider.h"
namespace jxx::com::sun::net::httpserver {::jxx::Ptr<HttpsServer>HttpsServer::create(){return ::jxx::com::sun::net::httpserver::spi::HttpServerProvider::provider()->createHttpsServer(nullptr,0);}::jxx::Ptr<HttpsServer>HttpsServer::create(const ::jxx::Ptr<::jxx::net::InetSocketAddress>&a,::jxx::lang::jint b){return ::jxx::com::sun::net::httpserver::spi::HttpServerProvider::provider()->createHttpsServer(a,b);}}
