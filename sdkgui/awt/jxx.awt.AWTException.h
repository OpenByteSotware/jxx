#pragma once

#include "lang/jxx.lang.Exception.h"
#include "lang/jxx.lang.String.h"

namespace jxx::awt
{
    class AWTException :
        public ::jxx::lang::ClassBase<AWTException, ::jxx::lang::Exception>
    {
    public:
        using JxxSuper = ::jxx::lang::Exception;
        using Super = ::jxx::lang::ClassBase<AWTException, JxxSuper>;

        explicit AWTException(const ::jxx::Ptr<::jxx::lang::String>& message);
    };
}
