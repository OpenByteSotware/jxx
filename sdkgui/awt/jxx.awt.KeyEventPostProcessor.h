#pragma once

#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::awt::event
{
    class KeyEvent;
}

namespace jxx::awt
{
    class KeyEventPostProcessor : public ::jxx::lang::InterfaceBase<
        KeyEventPostProcessor>
    {
    public:
        ~KeyEventPostProcessor() override = default;
        virtual ::jxx::lang::jbool postProcessKeyEvent(
            const ::jxx::Ptr<::jxx::awt::event::KeyEvent>& event) = 0;
    };
}
