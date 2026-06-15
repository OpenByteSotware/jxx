#pragma once

#include <stdexcept>

namespace jxx::net
{
    class SocketTimeoutException : public std::runtime_error
    {
    public:
        SocketTimeoutException();
        explicit SocketTimeoutException(const char* message);
        explicit SocketTimeoutException(const std::string& message);
        ~SocketTimeoutException() override = default;
    };
}
