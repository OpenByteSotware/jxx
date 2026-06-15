#pragma once

#include <stdexcept>

namespace jxx::net
{
    class UnknownServiceException : public std::runtime_error
    {
    public:
        UnknownServiceException();
        explicit UnknownServiceException(const char* message);
        explicit UnknownServiceException(const std::string& message);
        ~UnknownServiceException() override = default;
    };
}
