#pragma once

#include <stdexcept>

namespace jxx::net
{
    class UnknownHostException : public std::runtime_error
    {
    public:
        UnknownHostException();
        explicit UnknownHostException(const char* message);
        explicit UnknownHostException(const std::string& message);
        ~UnknownHostException() override = default;
    };
}
