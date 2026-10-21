#pragma once

#include "ext/net/jxx.ext.net.ServerSocketFactory.h"

namespace jxx::ext::net::internal {

class DefaultServerSocketFactory final
    : public ::jxx::lang::ClassBase<
          DefaultServerSocketFactory,
          ::jxx::ext::net::ServerSocketFactory> {
public:
    using JxxSuper = ::jxx::ext::net::ServerSocketFactory;
    using Super = ::jxx::lang::ClassBase<
        DefaultServerSocketFactory,
        JxxSuper>;

    ::jxx::Ptr<::jxx::net::ServerSocket>
    createServerSocket() override;

    ::jxx::Ptr<::jxx::net::ServerSocket>
    createServerSocket(::jxx::lang::jint port) override;

    ::jxx::Ptr<::jxx::net::ServerSocket>
    createServerSocket(
        ::jxx::lang::jint port,
        ::jxx::lang::jint backlog) override;

    ::jxx::Ptr<::jxx::net::ServerSocket>
    createServerSocket(
        ::jxx::lang::jint port,
        ::jxx::lang::jint backlog,
        const ::jxx::Ptr<::jxx::net::InetAddress>& address) override;
};

} // namespace jxx::ext::net::internal
