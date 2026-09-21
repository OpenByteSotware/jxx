#pragma once

#include "awt/print/jxx.awt.print.PrinterException.h"

namespace jxx::awt::print
{
    class PrinterAbortException : public ::jxx::lang::ClassBase<
        PrinterAbortException, PrinterException>
    {
    public:
        using JxxSuper = PrinterException;
        using Super = ::jxx::lang::ClassBase<PrinterAbortException,
            JxxSuper>;

        PrinterAbortException();
        explicit PrinterAbortException(
            const ::jxx::Ptr<::jxx::lang::String>& message);
    };
}
