#include "com/sun/net/httpserver/spi/jxx.com.sun.net.httpserver.spi.ClassInitialization.h"

#include <mutex>

#include "com/sun/net/httpserver/spi/jxx.com.sun.net.httpserver.spi.HttpServerProvider.h"

namespace jxx::com::sun::net::httpserver::spi {

void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
        (void)::jxx::lang::ClassBase<
            HttpServerProvider,
            ::jxx::lang::Object>::Class();
    });
}

} // namespace jxx::com::sun::net::httpserver::spi
