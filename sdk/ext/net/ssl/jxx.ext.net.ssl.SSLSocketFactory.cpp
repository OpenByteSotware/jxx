#include "ext/net/ssl/jxx.ext.net.ssl.SSLSocketFactory.h"

#include <mutex>

#include "ext/net/ssl/jxx.ext.net.ssl.SSLContext.h"
#include "lang/jxx.lang.UnsupportedOperationException.h"

namespace jxx::ext::net::ssl {
namespace {
std::once_flag defaultFactoryFlag;
::jxx::Ptr<::jxx::ext::net::SocketFactory> defaultFactory;
}

::jxx::Ptr<::jxx::ext::net::SocketFactory>
SSLSocketFactory::getDefault() {
    std::call_once(defaultFactoryFlag, []() {
        defaultFactory = SSLContext::getDefault()->getSocketFactory();
    });
    return defaultFactory;
}

::jxx::Ptr<::jxx::net::Socket> SSLSocketFactory::createSocket(
    const ::jxx::Ptr<::jxx::net::Socket>&,
    const ::jxx::Ptr<::jxx::io::InputStream>&,
    ::jxx::lang::jbool) {
    throw ::jxx::lang::UnsupportedOperationException();
}

} // namespace jxx::ext::net::ssl
