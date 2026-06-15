#pragma once

#include "net/jxx.net.SocketException.h"

namespace jxx::net
{
    class ConnectException : public SocketException
    {
    public:
        ConnectException();
        explicit ConnectException(const char* message);
        explicit ConnectException(const std::string& message);
        ~ConnectException() override = default;
    };
}
