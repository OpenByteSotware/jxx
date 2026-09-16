#pragma once

#include "lang/jxx.lang.UnsupportedOperationException.h"
#include "lang/jxx.lang.String.h"

namespace jxx::awt
{
    class HeadlessException :
        public ::jxx::lang::ClassBase<HeadlessException, ::jxx::lang::UnsupportedOperationException>
    {
    public:
        using JxxSuper = ::jxx::lang::UnsupportedOperationException;
        using Super = ::jxx::lang::ClassBase<HeadlessException, JxxSuper>;

        HeadlessException();
        explicit HeadlessException(const ::jxx::Ptr<::jxx::lang::String>& message);
    };
}
