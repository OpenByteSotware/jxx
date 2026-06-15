#pragma once

#include <stdexcept>

namespace jxx::net
{
    class MalformedURLException : public std::runtime_error
    {
    public:
        MalformedURLException();
        explicit MalformedURLException(const char* message);
        explicit MalformedURLException(const std::string& message);
        ~MalformedURLException() override = default;
    };
}
