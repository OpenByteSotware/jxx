#pragma once

#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::awt
{
    class PrintJob;

    class PrintGraphics : public ::jxx::lang::InterfaceBase<PrintGraphics>
    {
    public:
        ~PrintGraphics() override = default;
        virtual ::jxx::Ptr<PrintJob> getPrintJob() const = 0;
    };
}
