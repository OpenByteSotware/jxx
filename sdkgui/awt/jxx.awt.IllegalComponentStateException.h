#pragma once

#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.String.h"

namespace jxx::awt
{
    class IllegalComponentStateException :
        public ::jxx::lang::ClassBase<IllegalComponentStateException, ::jxx::lang::IllegalStateException>
    {
    public:
        using JxxSuper = ::jxx::lang::IllegalStateException;
        using Super = ::jxx::lang::ClassBase<IllegalComponentStateException, JxxSuper>;

        IllegalComponentStateException();
        explicit IllegalComponentStateException(const ::jxx::Ptr<::jxx::lang::String>& message);
    };
}
