#include "net/jxx.net.UnknownHostException.h"

namespace jxx::net
{
    UnknownHostException::UnknownHostException()
        : std::runtime_error("UnknownHostException")
    {
    }

    UnknownHostException::UnknownHostException(const char* message)
        : std::runtime_error(message ? message : "UnknownHostException")
    {
    }

    UnknownHostException::UnknownHostException(const std::string& message)
        : std::runtime_error(message)
    {
    }
}
