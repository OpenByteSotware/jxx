#pragma once

#include "awt/jxx.awt.Menu.h"

namespace jxx::awt
{
    class Component;

    class PopupMenu : public ::jxx::lang::ClassBase<PopupMenu, Menu>
    {
    public:
        using JxxSuper = Menu;
        using Super = ::jxx::lang::ClassBase<PopupMenu, JxxSuper>;

        PopupMenu();
        explicit PopupMenu(const ::jxx::Ptr<::jxx::lang::String>& label);
        ~PopupMenu() override = default;

        virtual void addNotify();
        virtual void show(const ::jxx::Ptr<Component>& origin,
            ::jxx::lang::jint x,
            ::jxx::lang::jint y);

    protected:
        ::jxx::Ptr<::jxx::lang::String> paramString() const;

    private:
        ::jxx::Ptr<Component> lastOrigin_;
        ::jxx::lang::jint lastX_ = 0;
        ::jxx::lang::jint lastY_ = 0;
    };
}
