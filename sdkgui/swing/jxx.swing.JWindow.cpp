#include "swing/jxx.swing.JWindow.h"

#include "gui/internal/jxx.gui.internal.WxDialogPeer.h"
#include "lang/jxx.lang.String.h"
#include "swing/jxx.swing.JLayeredPane.h"
#include "swing/jxx.swing.JRootPane.h"

namespace jxx::swing
{
    JWindow::JWindow() : Super()
    {
        initializeRootPane();
    }

    void JWindow::initializeRootPane()
    {
        rootPane_ = ::jxx::NEW<JRootPane>();
        ::jxx::awt::Window::add(rootPane_);
    }

    ::jxx::Ptr<JRootPane> JWindow::getRootPane() const
    { return rootPane_; }

    void JWindow::setContentPane(
        const ::jxx::Ptr<::jxx::awt::Container>& pane)
    { rootPane_->setContentPane(pane); }

    ::jxx::Ptr<::jxx::awt::Container> JWindow::getContentPane() const
    { return rootPane_->getContentPane(); }

    void JWindow::setLayeredPane(
        const ::jxx::Ptr<JLayeredPane>& pane)
    { rootPane_->setLayeredPane(pane); }

    ::jxx::Ptr<JLayeredPane> JWindow::getLayeredPane() const
    { return rootPane_->getLayeredPane(); }

    void JWindow::setGlassPane(
        const ::jxx::Ptr<::jxx::awt::Component>& pane)
    { rootPane_->setGlassPane(pane); }

    ::jxx::Ptr<::jxx::awt::Component> JWindow::getGlassPane() const
    { return rootPane_->getGlassPane(); }

    ::jxx::Ptr<::jxx::awt::Component> JWindow::add(
        const ::jxx::Ptr<::jxx::awt::Component>& component)
    { return getContentPane()->add(component); }

    void JWindow::remove(
        const ::jxx::Ptr<::jxx::awt::Component>& component)
    { getContentPane()->remove(component); }

    void JWindow::setLayout(
        const ::jxx::Ptr<::jxx::awt::LayoutManager>& layout)
    { getContentPane()->setLayout(layout); }

    void JWindow::validate()
    {
        rootPane_->validate();
        ::jxx::awt::Window::validate();
        const auto peer = ::jxx::CAST<
            ::jxx::gui::internal::WxDialogPeer>(nativeWindow_);
        if (peer != nullptr)
            peer->installComponents(rootPane_);
    }

    void JWindow::ensureNativeWindow()
    {
        if (nativeWindow_ != nullptr) return;
        auto title = ::jxx::NEW<::jxx::lang::String>("");
        auto peer = ::jxx::NEW<::jxx::gui::internal::WxDialogPeer>(
            title, false, true);
        nativeWindow_ = peer;
        configureNativeEvents();
        peer->installComponents(rootPane_);
    }
}
