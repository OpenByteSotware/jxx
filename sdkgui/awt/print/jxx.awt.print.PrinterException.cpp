#include "awt/print/jxx.awt.print.PrinterException.h"

namespace jxx::awt::print
{
    PrinterException::PrinterException() : Super()
    {
    }

    PrinterException::PrinterException(
        const ::jxx::Ptr<::jxx::lang::String>& message) : Super(message)
    {
    }
}
