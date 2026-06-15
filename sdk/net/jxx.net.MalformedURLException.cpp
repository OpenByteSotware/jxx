#include "net/jxx.net.MalformedURLException.h"

namespace jxx::net
{
    MalformedURLException::MalformedURLException()
        : std::runtime_error("MalformedURLException")
    {
    }

    MalformedURLException::MalformedURLException(const char* message)
        : std::runtime_error(message ? message : "MalformedURLException")
    {
    }

    MalformedURLException::MalformedURLException(const std::string& message)
        : std::runtime_error(message)
    {
    }
}
