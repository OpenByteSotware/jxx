#pragma once

#include "lang/jxx.lang.Exception.h"
#include "lang/jxx.lang.String.h"

namespace jxx::awt
{
    class FontFormatException :
        public ::jxx::lang::ClassBase<FontFormatException, ::jxx::lang::Exception>
    {
    public:
        using JxxSuper = ::jxx::lang::Exception;
        using Super = ::jxx::lang::ClassBase<FontFormatException, JxxSuper>;

        explicit FontFormatException(const ::jxx::Ptr<::jxx::lang::String>& message);
    };
}
