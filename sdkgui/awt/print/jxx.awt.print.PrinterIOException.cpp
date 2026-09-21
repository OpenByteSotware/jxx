#include "awt/print/jxx.awt.print.PrinterIOException.h"

#include "io/jxx.io.IOException.h"

namespace jxx::awt::print
{
    PrinterIOException::PrinterIOException(
        const ::jxx::Ptr<::jxx::io::IOException>& exception)
        : Super(exception == nullptr ? nullptr : exception->toString()),
          exception_(exception)
    {
    }

    ::jxx::Ptr<::jxx::io::IOException>
    PrinterIOException::getIOException() const
    {
        return exception_;
    }    
}
