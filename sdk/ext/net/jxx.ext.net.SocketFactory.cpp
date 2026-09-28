#include "ext/net/jxx.ext.net.SocketFactory.h"
#include "ext/net/internal.DefaultSocketFactory.h"
#include "net/jxx.net.Socket.h"
namespace jxx::ext::net {

::jxx::Ptr<SocketFactory>
SocketFactory::getDefault() {
    static const auto factory =
        ::jxx::NEW<
            ::jxx::ext::net::internal::
                DefaultSocketFactory>();
    return factory;
}

::jxx::Ptr<::jxx::net::Socket>
SocketFactory::createSocket() {
    return ::jxx::NEW<::jxx::net::Socket>();
}

} // namespace jxx::ext::net

