#pragma once
#include "util/jxx.util.EventListener.h"
namespace jxx::swing::event
{
    class MenuEvent;
    class MenuListener : public ::jxx::lang::InterfaceBase<MenuListener,
        ::jxx::util::EventListener>
    {
    public:
        ~MenuListener() override = default;
        virtual void menuSelected(const ::jxx::Ptr<MenuEvent>& event) = 0;
        virtual void menuDeselected(const ::jxx::Ptr<MenuEvent>& event) = 0;
        virtual void menuCanceled(const ::jxx::Ptr<MenuEvent>& event) = 0;
    };
}
