#include "swing/jxx.swing.JDialog.h"

#include "awt/event/jxx.awt.event.WindowEvent.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "swing/jxx.swing.JRootPane.h"

namespace jxx::swing
{
    JDialog::JDialog(const ::jxx::Ptr<::jxx::awt::Frame>& owner)
        : Super(owner) { initializeRootPane(); }
    JDialog::JDialog(const ::jxx::Ptr<::jxx::awt::Frame>& owner,
        const ::jxx::Ptr<::jxx::lang::String>& title)
        : Super(owner, title) { initializeRootPane(); }
    JDialog::JDialog(const ::jxx::Ptr<::jxx::awt::Frame>& owner,
        const ::jxx::Ptr<::jxx::lang::String>& title, ::jxx::lang::jbool modal)
        : Super(owner, title, modal) { initializeRootPane(); }
    void JDialog::initializeRootPane()
    {
        rootPane_ = ::jxx::NEW<JRootPane>();
        ::jxx::awt::Dialog::add(rootPane_);
    }
    ::jxx::Ptr<JRootPane> JDialog::getRootPane() const { return rootPane_; }
    void JDialog::setContentPane(const ::jxx::Ptr<::jxx::awt::Container>& pane) { rootPane_->setContentPane(pane); }
    ::jxx::Ptr<::jxx::awt::Container> JDialog::getContentPane() const { return rootPane_->getContentPane(); }
    void JDialog::setLayeredPane(const ::jxx::Ptr<JLayeredPane>& pane) { rootPane_->setLayeredPane(pane); }
    ::jxx::Ptr<JLayeredPane> JDialog::getLayeredPane() const { return rootPane_->getLayeredPane(); }
    void JDialog::setGlassPane(const ::jxx::Ptr<::jxx::awt::Component>& pane) { rootPane_->setGlassPane(pane); }
    ::jxx::Ptr<::jxx::awt::Component> JDialog::getGlassPane() const { return rootPane_->getGlassPane(); }
    void JDialog::setDefaultCloseOperation(::jxx::lang::jint value)
    {
        if (value < DO_NOTHING_ON_CLOSE || value > DISPOSE_ON_CLOSE)
            throw ::jxx::lang::IllegalArgumentException("operation");
        defaultCloseOperation_ = value;
    }
    ::jxx::lang::jint JDialog::getDefaultCloseOperation() const { return defaultCloseOperation_; }
    ::jxx::Ptr<::jxx::awt::Component> JDialog::add(const ::jxx::Ptr<::jxx::awt::Component>& component) { return getContentPane()->add(component); }
    void JDialog::remove(const ::jxx::Ptr<::jxx::awt::Component>& component) { getContentPane()->remove(component); }
    void JDialog::setLayout(const ::jxx::Ptr<::jxx::awt::LayoutManager>& layout) { getContentPane()->setLayout(layout); }
    void JDialog::validate() { rootPane_->validate(); ::jxx::awt::Dialog::validate(); }
    void JDialog::processWindowEvent(const ::jxx::Ptr<::jxx::awt::event::WindowEvent>& event)
    {
        ::jxx::awt::Dialog::processWindowEvent(event);
        if (event == nullptr || event->getID() != ::jxx::awt::event::WindowEvent::WINDOW_CLOSING) return;
        if (defaultCloseOperation_ == HIDE_ON_CLOSE) setVisible(false);
        else if (defaultCloseOperation_ == DISPOSE_ON_CLOSE) dispose();
    }
}
