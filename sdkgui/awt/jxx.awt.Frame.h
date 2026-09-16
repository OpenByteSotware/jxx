#pragma once

#include "awt/jxx.awt.Window.h"
#include "lang/jxx.lang.String.h"

namespace jxx::awt
{
    class MenuBar;
    class MenuComponent;

    class Frame : public ::jxx::lang::ClassBase<Frame, Window>
    {
    public:
        using JxxSuper = Window;
        using Super = ::jxx::lang::ClassBase<Frame, JxxSuper>;

        Frame();
        explicit Frame(const ::jxx::Ptr<::jxx::lang::String>& title);
        ~Frame() override = default;

        virtual void setTitle(
            const ::jxx::Ptr<::jxx::lang::String>& title);

        virtual ::jxx::Ptr<::jxx::lang::String> getTitle() const;

        virtual ::jxx::Ptr<MenuBar> getMenuBar() const;
        virtual void setMenuBar(const ::jxx::Ptr<MenuBar>& menuBar);
        virtual void remove(
            const ::jxx::Ptr<MenuComponent>& menuComponent);

    protected:
        void ensureNativeWindow() override;

    private:
        ::jxx::Ptr<::jxx::lang::String> title_;
        ::jxx::Ptr<MenuBar> menuBar_;
    };
}
