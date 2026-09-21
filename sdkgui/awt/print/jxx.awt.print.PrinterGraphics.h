#pragma once

#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::awt::print
{
    class PrinterJob;

    class PrinterGraphics : public ::jxx::lang::InterfaceBase<PrinterGraphics>
    {
    public:
        ~PrinterGraphics() override = default;

        virtual ::jxx::Ptr<PrinterJob> getPrinterJob() const = 0;
    };
}
