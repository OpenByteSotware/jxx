#include "net/jxx.net.ConnectException.h"

namespace jxx::net
{
    ConnectException::ConnectException()
        : SocketException("ConnectException")
    {
    }

    ConnectException::ConnectException(const char* message)
        : SocketException(message ? message : "ConnectException")
    {
    }

    ConnectException::ConnectException(const std::string& message)
        : SocketException(message)
    {
    }
}
