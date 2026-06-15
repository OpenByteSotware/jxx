#include "net/jxx.net.BindException.h"

namespace jxx::net
{
    BindException::BindException()
        : SocketException("BindException")
    {
    }

    BindException::BindException(const char* message)
        : SocketException(message ? message : "BindException")
    {
    }

    BindException::BindException(const std::string& message)
        : SocketException(message)
    {
    }
}
