#pragma once

#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::awt
{
    class Event;
    class Font;
    class MenuComponent;

    class MenuContainer : public ::jxx::lang::InterfaceBase<MenuContainer>
    {
    public:
        ~MenuContainer() override = default;
        virtual ::jxx::Ptr<Font> getFont() const = 0;
        virtual void remove(
            const ::jxx::Ptr<MenuComponent>& component) = 0;
        virtual ::jxx::lang::jbool postEvent(
            const ::jxx::Ptr<Event>& event) = 0;
    };
}
