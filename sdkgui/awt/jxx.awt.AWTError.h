#pragma once

#include "lang/jxx.lang.Error.h"
#include "lang/jxx.lang.String.h"

namespace jxx::awt
{
    class AWTError : public ::jxx::lang::ClassBase<AWTError, ::jxx::lang::Error>
    {
    public:
        using JxxSuper = ::jxx::lang::Error;
        using Super = ::jxx::lang::ClassBase<AWTError, JxxSuper>;

        explicit AWTError(const ::jxx::Ptr<::jxx::lang::String>& message);
    };
}
