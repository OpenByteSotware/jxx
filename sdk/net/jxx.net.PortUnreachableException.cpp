#include "net/jxx.net.PortUnreachableException.h"

namespace jxx::net
{
    PortUnreachableException::PortUnreachableException()
        : SocketException("PortUnreachableException")
    {
    }

    PortUnreachableException::PortUnreachableException(const char* message)
        : SocketException(message ? message : "PortUnreachableException")
    {
    }

    PortUnreachableException::PortUnreachableException(const std::string& message)
        : SocketException(message)
    {
    }
}
