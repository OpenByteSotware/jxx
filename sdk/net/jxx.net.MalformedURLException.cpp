#include "net/jxx.net.MalformedURLException.h"

namespace jxx::net
{
    MalformedURLException::MalformedURLException()
        : Super("MalformedURLException")
    {
    }

    MalformedURLException::MalformedURLException(const char* message)
        : Super(message ? message : "MalformedURLException")
    {
    }

    MalformedURLException::MalformedURLException(const std::string& message)
        : Super(message)
    {
    }
}
