#include "net/jxx.net.UnknownServiceException.h"

namespace jxx::net
{
    UnknownServiceException::UnknownServiceException()
        : std::runtime_error("UnknownServiceException")
    {
    }

    UnknownServiceException::UnknownServiceException(const char* message)
        : std::runtime_error(message ? message : "UnknownServiceException")
    {
    }

    UnknownServiceException::UnknownServiceException(const std::string& message)
        : std::runtime_error(message)
    {
    }
}
