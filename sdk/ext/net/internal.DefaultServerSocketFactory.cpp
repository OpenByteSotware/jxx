#include "ext/net/internal.DefaultServerSocketFactory.h"

#include "net/jxx.net.ServerSocket.h"

namespace jxx::ext::net::internal {

::jxx::Ptr<::jxx::net::ServerSocket>
DefaultServerSocketFactory::createServerSocket() {
    return ::jxx::NEW<::jxx::net::ServerSocket>();
}

::jxx::Ptr<::jxx::net::ServerSocket>
DefaultServerSocketFactory::createServerSocket(
    ::jxx::lang::jint port) {
    return ::jxx::NEW<::jxx::net::ServerSocket>(port);
}

::jxx::Ptr<::jxx::net::ServerSocket>
DefaultServerSocketFactory::createServerSocket(
    ::jxx::lang::jint port,
    ::jxx::lang::jint backlog) {
    return ::jxx::NEW<::jxx::net::ServerSocket>(port, backlog);
}

::jxx::Ptr<::jxx::net::ServerSocket>
DefaultServerSocketFactory::createServerSocket(
    ::jxx::lang::jint port,
    ::jxx::lang::jint backlog,
    const ::jxx::Ptr<::jxx::net::InetAddress>& address) {
    return ::jxx::NEW<::jxx::net::ServerSocket>(
        port,
        backlog,
        address);
}

} // namespace jxx::ext::net::internal
