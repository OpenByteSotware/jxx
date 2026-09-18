#pragma once

#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::awt
{
    class Component;
    class Container;
}

namespace jxx::swing
{
    class JLayeredPane;
    class JRootPane;

    class RootPaneContainer :
        public ::jxx::lang::InterfaceBase<RootPaneContainer>
    {
    public:
        ~RootPaneContainer() override = default;

        virtual ::jxx::Ptr<JRootPane> getRootPane() const = 0;
        virtual void setContentPane(
            const ::jxx::Ptr<::jxx::awt::Container>& contentPane) = 0;
        virtual ::jxx::Ptr<::jxx::awt::Container> getContentPane() const = 0;
        virtual void setLayeredPane(
            const ::jxx::Ptr<JLayeredPane>& layeredPane) = 0;
        virtual ::jxx::Ptr<JLayeredPane> getLayeredPane() const = 0;
        virtual void setGlassPane(
            const ::jxx::Ptr<::jxx::awt::Component>& glassPane) = 0;
        virtual ::jxx::Ptr<::jxx::awt::Component> getGlassPane() const = 0;
    };
}
