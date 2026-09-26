#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.DefaultHttpServerProvider.h"
#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.DefaultHttpServer.h"
#include "lang/jxx.lang.UnsupportedOperationException.h"

namespace jxx::com::sun::net::httpserver::internal {

::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpServer>
DefaultHttpServerProvider::createHttpServer(
    const ::jxx::Ptr<::jxx::net::InetSocketAddress>& address,
    ::jxx::lang::jint backlog)
{
    if (address == nullptr) {
        return ::jxx::NEW<DefaultHttpServer>();
    }

    return ::jxx::NEW<DefaultHttpServer>(address, backlog);
}

::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpsServer>
DefaultHttpServerProvider::createHttpsServer(
    const ::jxx::Ptr<::jxx::net::InetSocketAddress>&,
    ::jxx::lang::jint)
{
    // A plaintext fallback would violate the secure-server contract.
    // The concrete TLS server is supplied by the next HTTPS runtime stage.
    throw ::jxx::lang::UnsupportedOperationException();
}

} // namespace jxx::com::sun::net::httpserver::internal
