#pragma once

#include "awt/jxx.awt.Container.h"

namespace jxx::gui::internal
{
    class NativeWindow;
}

namespace jxx::awt
{
    class Window :
        public ::jxx::lang::ClassBase<Window, Container>
    {
    public:
        using JxxSuper = Container;
        using Super = ::jxx::lang::ClassBase<Window, JxxSuper>;

        Window();
        ~Window() override;

        virtual void pack();
        virtual void dispose();
        virtual void toFront();
        virtual void toBack();

        virtual ::jxx::lang::jbool isActive() const;
        virtual ::jxx::lang::jbool isDisplayable() const;

    protected:
        ::jxx::Ptr<::jxx::gui::internal::NativeWindow> nativeWindow_;
        ::jxx::lang::jbool active_;
        ::jxx::lang::jbool displayable_;
    };
}
