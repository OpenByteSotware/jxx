#include "net/jxx.net.BindException.h"

namespace jxx::net
{
    BindException::BindException()
        
    : Super("BindException")
    {
    }

    BindException::BindException(const char* message)
        
    : Super(message ? message : "BindException")
    {
    }

    BindException::BindException(const std::string& message)
        
    : Super(message)
    {
    }
}
