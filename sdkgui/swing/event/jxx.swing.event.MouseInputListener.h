#pragma once

#include "awt/event/jxx.awt.event.MouseListener.h"
#include "awt/event/jxx.awt.event.MouseMotionListener.h"

namespace jxx::swing::event
{
    class MouseInputListener : public ::jxx::lang::InterfaceBase<
        MouseInputListener,
        ::jxx::awt::event::MouseListener,
        ::jxx::awt::event::MouseMotionListener>
    {
    public:
        ~MouseInputListener() override = default;
    };
}
