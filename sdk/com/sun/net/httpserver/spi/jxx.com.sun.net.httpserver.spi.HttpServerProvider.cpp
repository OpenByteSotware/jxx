#include "com/sun/net/httpserver/spi/jxx.com.sun.net.httpserver.spi.HttpServerProvider.h"
#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.DefaultHttpServerProvider.h"

#include <mutex>

namespace jxx::com::sun::net::httpserver::spi {

::jxx::Ptr<HttpServerProvider> HttpServerProvider::provider()
{
    static std::once_flag once;
    static ::jxx::Ptr<HttpServerProvider> instance;

    std::call_once(once, [] {
        instance = ::jxx::NEW<
            ::jxx::com::sun::net::httpserver::internal::
                DefaultHttpServerProvider>();
    });

    return instance;
}

} // namespace jxx::com::sun::net::httpserver::spi
