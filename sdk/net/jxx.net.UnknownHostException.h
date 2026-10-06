#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "io/jxx.io.IOException.h"


namespace jxx::net
{
    class UnknownHostException : public ::jxx::lang::ClassBase<UnknownHostException, ::jxx::io::IOException>
    {
    public:
        using JxxSuper = ::jxx::io::IOException;
        using Super = ::jxx::lang::ClassBase<UnknownHostException, JxxSuper>;

        UnknownHostException();
        explicit UnknownHostException(const char* message);
        explicit UnknownHostException(const std::string& message);
        ~UnknownHostException() override = default;
    };
}
