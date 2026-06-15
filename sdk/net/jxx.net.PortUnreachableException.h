#pragma once

#include "net/jxx.net.SocketException.h"

namespace jxx::net
{
    class PortUnreachableException : public SocketException
    {
    public:
        PortUnreachableException();
        explicit PortUnreachableException(const char* message);
        explicit PortUnreachableException(const std::string& message);
        ~PortUnreachableException() override = default;
    };
}
