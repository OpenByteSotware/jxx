#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.buildin_array.h"

namespace jxx::awt
{
    class Component;
}

namespace jxx::awt::event
{
    class KeyEvent;
    class MouseEvent;
}

namespace jxx::swing
{
    class MenuSelectionManager;

    class MenuElement : public ::jxx::lang::InterfaceBase<MenuElement>
    {
    public:
        using MenuElementArray = ::jxx::Ptr<::jxx::lang::JxxArray<
            ::jxx::Ptr<MenuElement>, 1>>;

        ~MenuElement() override = default;

        virtual void processMouseEvent(
            const ::jxx::Ptr<::jxx::awt::event::MouseEvent>& event,
            const MenuElementArray& path,
            const ::jxx::Ptr<MenuSelectionManager>& manager) = 0;

        virtual void processKeyEvent(
            const ::jxx::Ptr<::jxx::awt::event::KeyEvent>& event,
            const MenuElementArray& path,
            const ::jxx::Ptr<MenuSelectionManager>& manager) = 0;

        virtual void menuSelectionChanged(
            ::jxx::lang::jbool isIncluded) = 0;

        virtual MenuElementArray getSubElements() const = 0;

        virtual ::jxx::Ptr<::jxx::awt::Component>
            getComponent() const = 0;
    };
}
