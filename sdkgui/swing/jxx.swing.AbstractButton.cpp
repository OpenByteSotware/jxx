#include "swing/jxx.swing.AbstractButton.h"

#include <algorithm>

#include "awt/event/jxx.awt.event.ActionEvent.h"
#include "awt/event/jxx.awt.event.ActionListener.h"
#include "gui/internal/jxx.gui.internal.NativeComponent.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "swing/jxx.swing.ButtonModel.h"
#include "swing/jxx.swing.DefaultButtonModel.h"

namespace jxx::swing
{
    namespace
    {
        ::jxx::Ptr<::jxx::lang::String> emptyText()
        { return ::jxx::NEW<::jxx::lang::String>(""); }
    }

    AbstractButton::AbstractButton() : AbstractButton(emptyText()) {}
    AbstractButton::AbstractButton(const ::jxx::Ptr<::jxx::lang::String>& value)
        : text_(value ? value : emptyText()),
          model_(::jxx::NEW<DefaultButtonModel>())
    {
    }

    ::jxx::Ptr<::jxx::lang::String> AbstractButton::getText() const { return text_; }
    void AbstractButton::setText(const ::jxx::Ptr<::jxx::lang::String>& value)
    { text_ = value ? value : emptyText(); if (nativeComponent_) nativeComponent_->setText(text_); invalidate(); }
    void AbstractButton::setActionCommand(const ::jxx::Ptr<::jxx::lang::String>& value)
    { actionCommand_ = value; if (model_) model_->setActionCommand(value); }
    ::jxx::Ptr<::jxx::lang::String> AbstractButton::getActionCommand() const
    { return actionCommand_ ? actionCommand_ : text_; }
    void AbstractButton::addActionListener(const ::jxx::Ptr<::jxx::awt::event::ActionListener>& value)
    { if (value && std::find(listeners_.begin(), listeners_.end(), value) == listeners_.end()) listeners_.push_back(value); }
    void AbstractButton::removeActionListener(const ::jxx::Ptr<::jxx::awt::event::ActionListener>& value)
    { listeners_.erase(std::remove(listeners_.begin(), listeners_.end(), value), listeners_.end()); }

    ::jxx::Ptr<ButtonModel> AbstractButton::getModel() const { return model_; }
    void AbstractButton::setModel(const ::jxx::Ptr<ButtonModel>& value)
    {
        if (!value) throw ::jxx::lang::NullPointerException("model");
        model_ = value;
        model_->setActionCommand(actionCommand_);
        model_->setEnabled(::jxx::awt::Component::isEnabled());
    }
    ::jxx::lang::jbool AbstractButton::isSelected() const { return model_ && model_->isSelected(); }
    void AbstractButton::setSelected(::jxx::lang::jbool value) { if (model_) model_->setSelected(value); }
    ::jxx::lang::jbool AbstractButton::isArmed() const { return model_ && model_->isArmed(); }
    ::jxx::lang::jbool AbstractButton::isPressed() const { return model_ && model_->isPressed(); }
    ::jxx::lang::jbool AbstractButton::isRollover() const { return model_ && model_->isRollover(); }
    void AbstractButton::setRolloverEnabled(::jxx::lang::jbool value) { rolloverEnabled_ = value; }
    ::jxx::lang::jbool AbstractButton::isRolloverEnabled() const { return rolloverEnabled_; }
    void AbstractButton::setMnemonic(::jxx::lang::jint value) { if (model_) model_->setMnemonic(value); }
    ::jxx::lang::jint AbstractButton::getMnemonic() const { return model_ ? model_->getMnemonic() : 0; }
    void AbstractButton::setEnabled(::jxx::lang::jbool value)
    { ::jxx::awt::Component::setEnabled(value); if (model_) model_->setEnabled(value); }

    void AbstractButton::doClick()
    {
        if (!model_ || !model_->isEnabled()) return;
        model_->setArmed(true);
        model_->setPressed(true);
        fireActionPerformed();
        model_->setPressed(false);
        model_->setArmed(false);
    }
    void AbstractButton::fireActionPerformed()
    { processActionEvent(::jxx::NEW<::jxx::awt::event::ActionEvent>(::jxx::CAST<::jxx::lang::Object>(thisPtr()), ::jxx::awt::event::ActionEvent::ACTION_PERFORMED, getActionCommand())); }
    void AbstractButton::processActionEvent(const ::jxx::Ptr<::jxx::awt::event::ActionEvent>& event)
    { const auto copy = listeners_; for (const auto& listener : copy) if (listener) listener->actionPerformed(event); }
}
