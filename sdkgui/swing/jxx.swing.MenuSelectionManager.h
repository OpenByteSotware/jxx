#pragma once

#include <vector>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "swing/jxx.swing.MenuElement.h"

namespace jxx::awt
{
    class Component;
    class Point;
}

namespace jxx::awt::event
{
    class KeyEvent;
    class MouseEvent;
}

namespace jxx::swing::event
{
    class ChangeListener;
}

namespace jxx::swing
{
    class MenuSelectionManager : public ::jxx::lang::ClassBase<
        MenuSelectionManager, ::jxx::lang::Object>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<MenuSelectionManager, JxxSuper>;
        using MenuElementArray = MenuElement::MenuElementArray;

        MenuSelectionManager() = default;
        ~MenuSelectionManager() override = default;

        static ::jxx::Ptr<MenuSelectionManager> defaultManager();

        void setSelectedPath(const MenuElementArray& path);
        MenuElementArray getSelectedPath() const;
        void clearSelectedPath();

        void processMouseEvent(
            const ::jxx::Ptr<::jxx::awt::event::MouseEvent>& event);
        void processKeyEvent(
            const ::jxx::Ptr<::jxx::awt::event::KeyEvent>& event);

        ::jxx::Ptr<::jxx::awt::Component> componentForPoint(
            const ::jxx::Ptr<::jxx::awt::Component>& source,
            const ::jxx::Ptr<::jxx::awt::Point>& sourcePoint) const;

        ::jxx::lang::jbool isComponentPartOfCurrentMenu(
            const ::jxx::Ptr<::jxx::awt::Component>& component) const;

        void addChangeListener(
            const ::jxx::Ptr<::jxx::swing::event::ChangeListener>& listener);
        void removeChangeListener(
            const ::jxx::Ptr<::jxx::swing::event::ChangeListener>& listener);

    protected:
        void fireStateChanged();

    private:
        std::vector<::jxx::Ptr<MenuElement>> selectedPath_;
        std::vector<::jxx::Ptr<::jxx::swing::event::ChangeListener>>
            changeListeners_;
    };
}
