#pragma once

#include <stdexcept>

namespace jxx::net
{
    class ProtocolException : public std::runtime_error
    {
    public:
        ProtocolException();
        explicit ProtocolException(const char* message);
        explicit ProtocolException(const std::string& message);
        ~ProtocolException() override = default;
    };
}
