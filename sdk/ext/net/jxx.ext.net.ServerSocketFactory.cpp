#include "ext/net/jxx.ext.net.ServerSocketFactory.h"

#include "ext/net/internal.DefaultServerSocketFactory.h"
#include "net/jxx.net.ServerSocket.h"

namespace jxx::ext::net {

::jxx::Ptr<ServerSocketFactory>
ServerSocketFactory::getDefault() {
    static const auto factory =
        ::jxx::NEW<
            ::jxx::ext::net::internal::
                DefaultServerSocketFactory>();
    return factory;
}

::jxx::Ptr<::jxx::net::ServerSocket>
ServerSocketFactory::createServerSocket() {
    return ::jxx::NEW<::jxx::net::ServerSocket>();
}

} // namespace jxx::ext::net
