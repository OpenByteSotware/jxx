#pragma once

#include "net/jxx.net.SocketException.h"

namespace jxx::net
{
    class BindException : public SocketException
    {
    public:
        BindException();
        explicit BindException(const char* message);
        explicit BindException(const std::string& message);
        ~BindException() override = default;
    };
}
