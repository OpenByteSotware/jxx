#include "awt/print/jxx.awt.print.PrinterAbortException.h"

namespace jxx::awt::print
{
    PrinterAbortException::PrinterAbortException() : Super()
    {
    }

    PrinterAbortException::PrinterAbortException(
        const ::jxx::Ptr<::jxx::lang::String>& message) : Super(message)
    {
    }
}
