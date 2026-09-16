#pragma once

#include "util/jxx.util.EventListener.h"

namespace jxx::awt
{
    class AWTEvent;
}

namespace jxx::awt::event
{
    class AWTEventListener :
        public ::jxx::lang::InterfaceBase<AWTEventListener,
            ::jxx::util::EventListener>
    {
    public:
        ~AWTEventListener() override = default;

        virtual void eventDispatched(
            const ::jxx::Ptr<::jxx::awt::AWTEvent>& event) = 0;
    };
}
