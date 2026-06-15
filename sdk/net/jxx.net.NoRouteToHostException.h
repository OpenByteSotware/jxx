#pragma once

#include "net/jxx.net.SocketException.h"

namespace jxx::net
{
    class NoRouteToHostException : public SocketException
    {
    public:
        NoRouteToHostException();
        explicit NoRouteToHostException(const char* message);
        explicit NoRouteToHostException(const std::string& message);
        ~NoRouteToHostException() override = default;
    };
}
