#pragma once

#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::awt
{
    class SecondaryLoop : public ::jxx::lang::InterfaceBase<SecondaryLoop>
    {
    public:
        ~SecondaryLoop() override = default;
        virtual ::jxx::lang::jbool enter() = 0;
        virtual ::jxx::lang::jbool exit() = 0;
    };
}
