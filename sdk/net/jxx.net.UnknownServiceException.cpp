#include "net/jxx.net.UnknownServiceException.h"

namespace jxx::net
{
    UnknownServiceException::UnknownServiceException()
        : Super("UnknownServiceException")
    {
    }

    UnknownServiceException::UnknownServiceException(const char* message)
        : Super(message ? message : "UnknownServiceException")
    {
    }

    UnknownServiceException::UnknownServiceException(const std::string& message)
        : Super(message)
    {
    }
}
