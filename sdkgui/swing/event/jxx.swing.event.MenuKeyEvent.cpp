#include "swing/event/jxx.swing.event.MenuKeyEvent.h"

namespace jxx::swing::event
{
    MenuKeyEvent::MenuKeyEvent(
        const ::jxx::Ptr<::jxx::awt::Component>& source,
        ::jxx::lang::jint id, ::jxx::lang::jlong when,
        ::jxx::lang::jint modifiers, ::jxx::lang::jint keyCode,
        ::jxx::lang::jchar keyChar,
        const MenuElementArray& path,
        const ::jxx::Ptr<::jxx::swing::MenuSelectionManager>& manager)
        : Super(source, id, when, modifiers, keyCode, keyChar),
          path_(path), manager_(manager) {}

    MenuKeyEvent::MenuElementArray MenuKeyEvent::getPath() const
    {
        return path_;
    }

    ::jxx::Ptr<::jxx::swing::MenuSelectionManager>
    MenuKeyEvent::getMenuSelectionManager() const
    {
        return manager_;
    }
}
