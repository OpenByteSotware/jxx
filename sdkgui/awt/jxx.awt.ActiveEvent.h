#pragma once

#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::awt
{
    class ActiveEvent : public ::jxx::lang::InterfaceBase<ActiveEvent>
    {
    public:
        ~ActiveEvent() override = default;
        virtual void dispatch() = 0;
    };
}
