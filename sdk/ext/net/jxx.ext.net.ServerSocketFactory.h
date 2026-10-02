#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::net {
class InetAddress;
class ServerSocket;
}

namespace jxx::ext::net {

class ServerSocketFactory
    : public ::jxx::lang::ClassBase<
          ServerSocketFactory,
          ::jxx::lang::Object> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<
        ServerSocketFactory,
        JxxSuper>;

    ~ServerSocketFactory() override = default;

    static ::jxx::Ptr<ServerSocketFactory> getDefault();

    virtual ::jxx::Ptr<::jxx::net::ServerSocket>
    createServerSocket();

    virtual ::jxx::Ptr<::jxx::net::ServerSocket>
    createServerSocket(::jxx::lang::jint port) = 0;

    virtual ::jxx::Ptr<::jxx::net::ServerSocket>
    createServerSocket(
        ::jxx::lang::jint port,
        ::jxx::lang::jint backlog) = 0;

    virtual ::jxx::Ptr<::jxx::net::ServerSocket>
    createServerSocket(
        ::jxx::lang::jint port,
        ::jxx::lang::jint backlog,
        const ::jxx::Ptr<::jxx::net::InetAddress>& address) = 0;

protected:
    ServerSocketFactory() = default;
};

} // namespace jxx::ext::net
