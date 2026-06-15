#pragma once

#include <stdexcept>

namespace jxx::net
{
    class SocketException : public std::runtime_error
    {
    public:
        SocketException();
        explicit SocketException(const char* message);
        explicit SocketException(const std::string& message);
        ~SocketException() override = default;
    };
}
