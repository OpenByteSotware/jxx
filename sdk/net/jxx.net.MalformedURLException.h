#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "io/jxx.io.IOException.h"


namespace jxx::net
{
    class MalformedURLException : public ::jxx::lang::ClassBase<MalformedURLException, ::jxx::io::IOException>
    {
    public:
        using JxxSuper = ::jxx::io::IOException;
        using Super = ::jxx::lang::ClassBase<MalformedURLException, JxxSuper>;

        MalformedURLException();
        explicit MalformedURLException(const char* message);
        explicit MalformedURLException(const std::string& message);
        ~MalformedURLException() override = default;
    };
}
