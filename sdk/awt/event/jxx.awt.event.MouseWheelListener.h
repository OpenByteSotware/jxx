#pragma once

#include "util/jxx.util.EventListener.h"

namespace jxx::awt::event
{
    class MouseWheelEvent;

    class MouseWheelListener :
        public ::jxx::lang::InterfaceBase<MouseWheelListener,
            ::jxx::util::EventListener>
    {
    public:
        ~MouseWheelListener() override = default;
        virtual void mouseWheelMoved(
            const ::jxx::Ptr<MouseWheelEvent>& event) = 0;
    };
}
