#include "swing/jxx.swing.JFrame.h"

#include "awt/event/jxx.awt.event.WindowEvent.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "swing/jxx.swing.JRootPane.h"
#include "swing/jxx.swing.JMenuBar.h"
#include "gui/internal/jxx.gui.internal.NativeWindow.h"

namespace jxx::swing
{
    JFrame::JFrame() : Super() { initializeRootPane(); }
    JFrame::JFrame(const ::jxx::Ptr<::jxx::lang::String>& title)
        : Super(title) { initializeRootPane(); }
    void JFrame::initializeRootPane()
    {
        rootPane_ = ::jxx::NEW<JRootPane>();
        ::jxx::awt::Frame::add(rootPane_);
    }
    ::jxx::Ptr<JRootPane> JFrame::getRootPane() const { return rootPane_; }
    void JFrame::setContentPane(const ::jxx::Ptr<::jxx::awt::Container>& pane) { rootPane_->setContentPane(pane); }
    ::jxx::Ptr<::jxx::awt::Container> JFrame::getContentPane() const { return rootPane_->getContentPane(); }
    void JFrame::setLayeredPane(const ::jxx::Ptr<JLayeredPane>& pane) { rootPane_->setLayeredPane(pane); }
    ::jxx::Ptr<JLayeredPane> JFrame::getLayeredPane() const { return rootPane_->getLayeredPane(); }
    void JFrame::setGlassPane(const ::jxx::Ptr<::jxx::awt::Component>& pane) { rootPane_->setGlassPane(pane); }
    ::jxx::Ptr<::jxx::awt::Component> JFrame::getGlassPane() const { return rootPane_->getGlassPane(); }
    void JFrame::setJMenuBar(const ::jxx::Ptr<JMenuBar>& value)
    {
        rootPane_->setJMenuBar(value);
        if (nativeWindow_ != nullptr) nativeWindow_->setSwingMenuBar(value);
    }
    ::jxx::Ptr<JMenuBar> JFrame::getJMenuBar() const { return rootPane_->getJMenuBar(); }
    void JFrame::setDefaultCloseOperation(::jxx::lang::jint value)
    {
        if (value < DO_NOTHING_ON_CLOSE || value > EXIT_ON_CLOSE)
            throw ::jxx::lang::IllegalArgumentException("operation");
        defaultCloseOperation_ = value;
    }
    ::jxx::lang::jint JFrame::getDefaultCloseOperation() const { return defaultCloseOperation_; }
    ::jxx::Ptr<::jxx::awt::Component> JFrame::add(const ::jxx::Ptr<::jxx::awt::Component>& component) { return getContentPane()->add(component); }
    void JFrame::remove(const ::jxx::Ptr<::jxx::awt::Component>& component) { getContentPane()->remove(component); }
    void JFrame::setLayout(const ::jxx::Ptr<::jxx::awt::LayoutManager>& layout) { getContentPane()->setLayout(layout); }
    void JFrame::validate() { rootPane_->validate(); ::jxx::awt::Frame::validate(); }
    void JFrame::processWindowEvent(const ::jxx::Ptr<::jxx::awt::event::WindowEvent>& event)
    {
        ::jxx::awt::Frame::processWindowEvent(event);
        if (event == nullptr || event->getID() != ::jxx::awt::event::WindowEvent::WINDOW_CLOSING) return;
        if (defaultCloseOperation_ == HIDE_ON_CLOSE) setVisible(false);
        else if (defaultCloseOperation_ == DISPOSE_ON_CLOSE) dispose();
        else if (defaultCloseOperation_ == EXIT_ON_CLOSE) dispose();
    }
}
