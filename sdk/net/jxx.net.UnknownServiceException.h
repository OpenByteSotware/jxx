#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "io/jxx.io.IOException.h"


namespace jxx::net
{
    class UnknownServiceException : public ::jxx::lang::ClassBase<UnknownServiceException, ::jxx::io::IOException>
    {
    public:
        using JxxSuper = ::jxx::io::IOException;
        using Super = ::jxx::lang::ClassBase<UnknownServiceException, JxxSuper>;

        UnknownServiceException();
        explicit UnknownServiceException(const char* message);
        explicit UnknownServiceException(const std::string& message);
        ~UnknownServiceException() override = default;
    };
}
