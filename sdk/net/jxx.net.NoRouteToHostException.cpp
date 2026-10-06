#include "net/jxx.net.NoRouteToHostException.h"

namespace jxx::net
{
    NoRouteToHostException::NoRouteToHostException()
        : Super("NoRouteToHostException")
    {
    }

    NoRouteToHostException::NoRouteToHostException(const char* message)
        : Super(message ? message : "NoRouteToHostException")
    {
    }

    NoRouteToHostException::NoRouteToHostException(const std::string& message)
        : Super(message)
    {
    }
}
