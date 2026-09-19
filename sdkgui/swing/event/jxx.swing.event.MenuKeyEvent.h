#pragma once

#include "awt/event/jxx.awt.event.KeyEvent.h"
#include "lang/jxx.lang.buildin_array.h"

namespace jxx::swing
{
    class MenuElement;
    class MenuSelectionManager;
}

namespace jxx::swing::event
{
    class MenuKeyEvent : public ::jxx::lang::ClassBase<
        MenuKeyEvent, ::jxx::awt::event::KeyEvent>
    {
    public:
        using JxxSuper = ::jxx::awt::event::KeyEvent;
        using Super = ::jxx::lang::ClassBase<MenuKeyEvent, JxxSuper>;
        using MenuElementArray = ::jxx::Ptr<::jxx::lang::JxxArray<
            ::jxx::Ptr<::jxx::swing::MenuElement>, 1>>;

        MenuKeyEvent(
            const ::jxx::Ptr<::jxx::awt::Component>& source,
            ::jxx::lang::jint id, ::jxx::lang::jlong when,
            ::jxx::lang::jint modifiers, ::jxx::lang::jint keyCode,
            ::jxx::lang::jchar keyChar,
            const MenuElementArray& path,
            const ::jxx::Ptr<::jxx::swing::MenuSelectionManager>& manager);

        MenuElementArray getPath() const;
        ::jxx::Ptr<::jxx::swing::MenuSelectionManager>
            getMenuSelectionManager() const;

    private:
        MenuElementArray path_;
        ::jxx::Ptr<::jxx::swing::MenuSelectionManager> manager_;
    };
}
