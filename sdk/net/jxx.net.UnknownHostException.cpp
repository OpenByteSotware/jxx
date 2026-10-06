#include "net/jxx.net.UnknownHostException.h"

namespace jxx::net
{
    UnknownHostException::UnknownHostException()
        : Super("UnknownHostException")
    {
    }

    UnknownHostException::UnknownHostException(const char* message)
        : Super(message ? message : "UnknownHostException")
    {
    }

    UnknownHostException::UnknownHostException(const std::string& message)
        : Super(message)
    {
    }
}
