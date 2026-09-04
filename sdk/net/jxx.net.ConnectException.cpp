#include "net/jxx.net.ConnectException.h"

namespace jxx::net
{
    ConnectException::ConnectException()
        
    : Super("ConnectException")
    {
    }

    ConnectException::ConnectException(const char* message)
        
    : Super(message ? message : "ConnectException")
    {
    }

    ConnectException::ConnectException(const std::string& message)
        
    : Super(message)
    {
    }
}
