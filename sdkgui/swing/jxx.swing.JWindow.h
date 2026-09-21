#pragma once

#include "awt/jxx.awt.Window.h"
#include "swing/jxx.swing.RootPaneContainer.h"

namespace jxx::swing
{
    class JWindow : public ::jxx::lang::ClassBase<JWindow,
        ::jxx::awt::Window, RootPaneContainer>
    {
    public:
        using JxxSuper = ::jxx::awt::Window;
        using Super = ::jxx::lang::ClassBase<JWindow,
            JxxSuper, RootPaneContainer>;

        JWindow();
        ~JWindow() override = default;

        ::jxx::Ptr<JRootPane> getRootPane() const override;
        void setContentPane(
            const ::jxx::Ptr<::jxx::awt::Container>& pane) override;
        ::jxx::Ptr<::jxx::awt::Container> getContentPane() const override;
        void setLayeredPane(
            const ::jxx::Ptr<JLayeredPane>& pane) override;
        ::jxx::Ptr<JLayeredPane> getLayeredPane() const override;
        void setGlassPane(
            const ::jxx::Ptr<::jxx::awt::Component>& pane) override;
        ::jxx::Ptr<::jxx::awt::Component> getGlassPane() const override;

        ::jxx::Ptr<::jxx::awt::Component> add(
            const ::jxx::Ptr<::jxx::awt::Component>& component) override;
        void remove(
            const ::jxx::Ptr<::jxx::awt::Component>& component) override;
        void setLayout(
            const ::jxx::Ptr<::jxx::awt::LayoutManager>& layout) override;
        void validate() override;

    protected:
        void ensureNativeWindow() override;

    private:
        void initializeRootPane();
        ::jxx::Ptr<JRootPane> rootPane_;
    };
}
