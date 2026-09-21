#pragma once

#include "lang/jxx.lang.Exception.h"
#include "lang/jxx.lang.String.h"

namespace jxx::awt::print
{
    class PrinterException : public ::jxx::lang::ClassBase<
        PrinterException, ::jxx::lang::Exception>
    {
    public:
        using JxxSuper = ::jxx::lang::Exception;
        using Super = ::jxx::lang::ClassBase<PrinterException, JxxSuper>;

        PrinterException();
        explicit PrinterException(
            const ::jxx::Ptr<::jxx::lang::String>& message);
    };
}
