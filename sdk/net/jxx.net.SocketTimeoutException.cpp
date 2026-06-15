#include "net/jxx.net.SocketTimeoutException.h"

namespace jxx::net
{
    SocketTimeoutException::SocketTimeoutException()
        : std::runtime_error("SocketTimeoutException")
    {
    }

    SocketTimeoutException::SocketTimeoutException(const char* message)
        : std::runtime_error(message ? message : "SocketTimeoutException")
    {
    }

    SocketTimeoutException::SocketTimeoutException(const std::string& message)
        : std::runtime_error(message)
    {
    }
}
