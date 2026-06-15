#include "net/jxx.net.ProtocolException.h"

namespace jxx::net
{
    ProtocolException::ProtocolException()
        : std::runtime_error("ProtocolException")
    {
    }

    ProtocolException::ProtocolException(const char* message)
        : std::runtime_error(message ? message : "ProtocolException")
    {
    }

    ProtocolException::ProtocolException(const std::string& message)
        : std::runtime_error(message)
    {
    }
}
