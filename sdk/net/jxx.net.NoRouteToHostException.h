#pragma once

#include "lang/jxx.lang.ClassInfo.h"

#include "net/jxx.net.SocketException.h"

namespace jxx::net
{
    class NoRouteToHostException : public ::jxx::lang::ClassBase<NoRouteToHostException, SocketException> {
    public:
        using JxxSuper = SocketException;
        using Super = ::jxx::lang::ClassBase<NoRouteToHostException, JxxSuper>;

        NoRouteToHostException();
        explicit NoRouteToHostException(const char* message);
        explicit NoRouteToHostException(const std::string& message);
        ~NoRouteToHostException() override = default;
    };
}
