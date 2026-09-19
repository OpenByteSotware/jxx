#pragma once

#include "util/jxx.util.EventListener.h"

namespace jxx::swing::event
{
    class MenuKeyEvent;
    class MenuKeyListener : public ::jxx::lang::InterfaceBase<
        MenuKeyListener, ::jxx::util::EventListener>
    {
    public:
        ~MenuKeyListener() override = default;
        virtual void menuKeyTyped(
            const ::jxx::Ptr<MenuKeyEvent>& event) = 0;
        virtual void menuKeyPressed(
            const ::jxx::Ptr<MenuKeyEvent>& event) = 0;
        virtual void menuKeyReleased(
            const ::jxx::Ptr<MenuKeyEvent>& event) = 0;
    };
}
