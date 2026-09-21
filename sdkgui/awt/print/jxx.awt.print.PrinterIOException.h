#pragma once

#include "awt/print/jxx.awt.print.PrinterException.h"

namespace jxx::io { class IOException; }

namespace jxx::awt::print
{
    class PrinterIOException : public ::jxx::lang::ClassBase<
        PrinterIOException, PrinterException>
    {
    public:
        using JxxSuper = PrinterException;
        using Super = ::jxx::lang::ClassBase<PrinterIOException, JxxSuper>;

        explicit PrinterIOException(
            const ::jxx::Ptr<::jxx::io::IOException>& exception);
        ::jxx::Ptr<::jxx::io::IOException> getIOException() const;        

    private:
        ::jxx::Ptr<::jxx::io::IOException> exception_;
    };
}
