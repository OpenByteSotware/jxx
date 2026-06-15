#include "net/jxx.net.SocketException.h"

namespace jxx::net
{
    SocketException::SocketException()
        : std::runtime_error("SocketException")
    {
    }

    SocketException::SocketException(const char* message)
        : std::runtime_error(message ? message : "SocketException")
    {
    }

    SocketException::SocketException(const std::string& message)
        : std::runtime_error(message)
    {
    }
}
