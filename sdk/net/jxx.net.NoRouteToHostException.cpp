#include "net/jxx.net.NoRouteToHostException.h"

namespace jxx::net
{
    NoRouteToHostException::NoRouteToHostException()
        : SocketException("NoRouteToHostException")
    {
    }

    NoRouteToHostException::NoRouteToHostException(const char* message)
        : SocketException(message ? message : "NoRouteToHostException")
    {
    }

    NoRouteToHostException::NoRouteToHostException(const std::string& message)
        : SocketException(message)
    {
    }
}
