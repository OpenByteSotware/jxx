#pragma once

#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::awt::event
{
    class KeyEvent;
}

namespace jxx::awt
{
    class KeyEventDispatcher : public ::jxx::lang::InterfaceBase<
        KeyEventDispatcher>
    {
    public:
        ~KeyEventDispatcher() override = default;
        virtual ::jxx::lang::jbool dispatchKeyEvent(
            const ::jxx::Ptr<::jxx::awt::event::KeyEvent>& event) = 0;
    };
}
