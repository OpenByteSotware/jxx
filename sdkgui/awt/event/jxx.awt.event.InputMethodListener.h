#pragma once

#include "util/jxx.util.EventListener.h"

namespace jxx::awt::event
{
    class InputMethodEvent;

    class InputMethodListener :
        public ::jxx::lang::InterfaceBase<InputMethodListener,
            ::jxx::util::EventListener>
    {
    public:
        ~InputMethodListener() override = default;

        virtual void inputMethodTextChanged(
            const ::jxx::Ptr<InputMethodEvent>& event) = 0;

        virtual void caretPositionChanged(
            const ::jxx::Ptr<InputMethodEvent>& event) = 0;
    };
}
