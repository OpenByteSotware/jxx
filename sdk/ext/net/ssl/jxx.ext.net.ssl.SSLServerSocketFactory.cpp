#include "ext/net/ssl/jxx.ext.net.ssl.SSLServerSocketFactory.h"

#include <mutex>

#include "ext/net/ssl/jxx.ext.net.ssl.SSLContext.h"

namespace jxx::ext::net::ssl {
namespace {
std::once_flag defaultServerFactoryFlag;
::jxx::Ptr<::jxx::ext::net::ServerSocketFactory>
defaultServerFactory;
}

::jxx::Ptr<::jxx::ext::net::ServerSocketFactory>
SSLServerSocketFactory::getDefault() {
    std::call_once(defaultServerFactoryFlag, []() {
        defaultServerFactory =
            SSLContext::getDefault()
                ->getServerSocketFactory();
    });
    return defaultServerFactory;
}

} // namespace jxx::ext::net::ssl
