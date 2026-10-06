#include "net/jxx.net.PortUnreachableException.h"

namespace jxx::net
{
    PortUnreachableException::PortUnreachableException()
        : Super("PortUnreachableException")
    {
    }

    PortUnreachableException::PortUnreachableException(const char* message)
        : Super(message ? message : "PortUnreachableException")
    {
    }

    PortUnreachableException::PortUnreachableException(const std::string& message)
        : Super(message)
    {
    }
}
