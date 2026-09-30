#include <mutex>

#include "com/sun/net/httpserver/spi/jxx.com.sun.net.httpserver.spi.HttpServerProvider.h"
#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.DefaultHttpServerProvider.h"
#include "lang/jxx.lang.Exceptions.h"

namespace jxx::com::sun::net::httpserver::spi {
namespace {
std::mutex providerMutex;
::jxx::Ptr<HttpServerProvider> providerInstance;
::jxx::lang::jbool providerResolved = false;
}

::jxx::Ptr<HttpServerProvider> HttpServerProvider::provider()
{
    std::lock_guard<std::mutex> lock(providerMutex);
    if (providerInstance == nullptr) {
        providerInstance = ::jxx::NEW<
            ::jxx::com::sun::net::httpserver::internal::DefaultHttpServerProvider>();
    }
    providerResolved = true;
    return providerInstance;
}

void HttpServerProvider::setProvider(
    const ::jxx::Ptr<HttpServerProvider>& value)
{
    if (value == nullptr) throw ::jxx::lang::NullPointerException();
    std::lock_guard<std::mutex> lock(providerMutex);
    if (providerResolved) throw ::jxx::lang::IllegalStateException();
    providerInstance = value;
}

} // namespace jxx::com::sun::net::httpserver::spi
