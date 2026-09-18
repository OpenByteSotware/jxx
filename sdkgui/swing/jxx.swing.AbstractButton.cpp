#include "swing/jxx.swing.AbstractButton.h"
#include <algorithm>
#include "awt/event/jxx.awt.event.ActionEvent.h"
#include "awt/event/jxx.awt.event.ActionListener.h"
#include "gui/internal/jxx.gui.internal.NativeComponent.h"
namespace jxx::swing
{
    namespace { ::jxx::Ptr<::jxx::lang::String> emptyText(){ return ::jxx::NEW<::jxx::lang::String>(""); } }
    AbstractButton::AbstractButton() : AbstractButton(emptyText()) {}
    AbstractButton::AbstractButton(const ::jxx::Ptr<::jxx::lang::String>& value) : text_(value ? value : emptyText()) {}
    ::jxx::Ptr<::jxx::lang::String> AbstractButton::getText() const { return text_; }
    void AbstractButton::setText(const ::jxx::Ptr<::jxx::lang::String>& value)
    { text_ = value ? value : emptyText(); if (nativeComponent_) nativeComponent_->setText(text_); invalidate(); }
    void AbstractButton::setActionCommand(const ::jxx::Ptr<::jxx::lang::String>& value) { actionCommand_ = value; }
    ::jxx::Ptr<::jxx::lang::String> AbstractButton::getActionCommand() const { return actionCommand_ ? actionCommand_ : text_; }
    void AbstractButton::addActionListener(const ::jxx::Ptr<::jxx::awt::event::ActionListener>& value)
    { if (value && std::find(listeners_.begin(), listeners_.end(), value) == listeners_.end()) listeners_.push_back(value); }
    void AbstractButton::removeActionListener(const ::jxx::Ptr<::jxx::awt::event::ActionListener>& value)
    { listeners_.erase(std::remove(listeners_.begin(), listeners_.end(), value), listeners_.end()); }
    void AbstractButton::doClick() { fireActionPerformed(); }
    void AbstractButton::fireActionPerformed()
    { processActionEvent(::jxx::NEW<::jxx::awt::event::ActionEvent>(::jxx::CAST<::jxx::lang::Object>(thisPtr()), ::jxx::awt::event::ActionEvent::ACTION_PERFORMED, getActionCommand())); }
    void AbstractButton::processActionEvent(const ::jxx::Ptr<::jxx::awt::event::ActionEvent>& event)
    { const auto copy = listeners_; for (const auto& listener : copy) if (listener) listener->actionPerformed(event); }
}
