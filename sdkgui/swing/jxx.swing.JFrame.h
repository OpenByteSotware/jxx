#pragma once

#include "awt/jxx.awt.Frame.h"
#include "swing/jxx.swing.RootPaneContainer.h"
#include "swing/jxx.swing.WindowConstants.h"

namespace jxx::swing
{
    class JFrame : public ::jxx::lang::ClassBase<JFrame,
        ::jxx::awt::Frame, RootPaneContainer, WindowConstants>
    {
    public:
        using JxxSuper = ::jxx::awt::Frame;
        using Super = ::jxx::lang::ClassBase<JFrame, JxxSuper,
            RootPaneContainer, WindowConstants>;

        JFrame();
        explicit JFrame(const ::jxx::Ptr<::jxx::lang::String>& title);
        ~JFrame() override = default;

        ::jxx::Ptr<JRootPane> getRootPane() const override;
        void setContentPane(const ::jxx::Ptr<::jxx::awt::Container>& pane) override;
        ::jxx::Ptr<::jxx::awt::Container> getContentPane() const override;
        void setLayeredPane(const ::jxx::Ptr<JLayeredPane>& pane) override;
        ::jxx::Ptr<JLayeredPane> getLayeredPane() const override;
        void setGlassPane(const ::jxx::Ptr<::jxx::awt::Component>& pane) override;
        ::jxx::Ptr<::jxx::awt::Component> getGlassPane() const override;
        void setDefaultCloseOperation(::jxx::lang::jint operation);
        ::jxx::lang::jint getDefaultCloseOperation() const;

        ::jxx::Ptr<::jxx::awt::Component> add(
            const ::jxx::Ptr<::jxx::awt::Component>& component) override;
        void remove(const ::jxx::Ptr<::jxx::awt::Component>& component) override;
        void setLayout(const ::jxx::Ptr<::jxx::awt::LayoutManager>& layout) override;
        void validate() override;

    protected:
        void processWindowEvent(
            const ::jxx::Ptr<::jxx::awt::event::WindowEvent>& event) override;

    private:
        void initializeRootPane();
        ::jxx::Ptr<JRootPane> rootPane_;
        ::jxx::lang::jint defaultCloseOperation_ = HIDE_ON_CLOSE;
    };
}
