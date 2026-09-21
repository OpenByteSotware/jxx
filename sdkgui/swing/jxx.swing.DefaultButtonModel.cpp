#include "swing/jxx.swing.DefaultButtonModel.h"

#include <algorithm>

#include "awt/event/jxx.awt.event.ActionEvent.h"
#include "awt/event/jxx.awt.event.ItemEvent.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "swing/event/jxx.swing.event.ChangeEvent.h"

namespace jxx::swing
{
    DefaultButtonModel::DefaultButtonModel() = default;

    ::jxx::lang::jbool DefaultButtonModel::isArmed() const { return armed_; }
    ::jxx::lang::jbool DefaultButtonModel::isSelected() const { return selected_; }
    ::jxx::lang::jbool DefaultButtonModel::isEnabled() const { return enabled_; }
    ::jxx::lang::jbool DefaultButtonModel::isPressed() const { return pressed_; }
    ::jxx::lang::jbool DefaultButtonModel::isRollover() const { return rollover_; }
    ::jxx::lang::jint DefaultButtonModel::getMnemonic() const { return mnemonic_; }
    ::jxx::Ptr<::jxx::lang::String> DefaultButtonModel::getActionCommand() const { return actionCommand_; }

    void DefaultButtonModel::setArmed(::jxx::lang::jbool value)
    { if (armed_ != value) { armed_ = value; fireStateChanged(); } }
    void DefaultButtonModel::setSelected(::jxx::lang::jbool value)
    { if (selected_ != value) { selected_ = value; fireItemStateChanged(); fireStateChanged(); } }
    void DefaultButtonModel::setEnabled(::jxx::lang::jbool value)
    { if (enabled_ != value) { enabled_ = value; if (!enabled_) { armed_ = false; pressed_ = false; } fireStateChanged(); } }
    void DefaultButtonModel::setPressed(::jxx::lang::jbool value)
    {
        if (!enabled_ || pressed_ == value) return;
        const auto fireAction = pressed_ && !value && armed_;
        pressed_ = value;
        fireStateChanged();
        if (fireAction) fireActionPerformed();
    }
    void DefaultButtonModel::setRollover(::jxx::lang::jbool value)
    { if (rollover_ != value) { rollover_ = value; fireStateChanged(); } }
    void DefaultButtonModel::setMnemonic(::jxx::lang::jint value)
    { if (mnemonic_ != value) { mnemonic_ = value; fireStateChanged(); } }
    void DefaultButtonModel::setActionCommand(const ::jxx::Ptr<::jxx::lang::String>& value)
    { actionCommand_ = value; }
    void DefaultButtonModel::setGroup(const ::jxx::Ptr<ButtonGroup>& value) { group_ = value; }

    ::jxx::lang::ObjectArray DefaultButtonModel::getSelectedObjects() const { return nullptr; }

    void DefaultButtonModel::addActionListener(const ::jxx::Ptr<::jxx::awt::event::ActionListener>& value)
    { if (value) actionListeners_.push_back(value); }
    void DefaultButtonModel::removeActionListener(const ::jxx::Ptr<::jxx::awt::event::ActionListener>& value)
    { actionListeners_.erase(std::remove(actionListeners_.begin(), actionListeners_.end(), value), actionListeners_.end()); }
    void DefaultButtonModel::addItemListener(const ::jxx::Ptr<::jxx::awt::event::ItemListener>& value)
    { if (value) itemListeners_.push_back(value); }
    void DefaultButtonModel::removeItemListener(const ::jxx::Ptr<::jxx::awt::event::ItemListener>& value)
    { itemListeners_.erase(std::remove(itemListeners_.begin(), itemListeners_.end(), value), itemListeners_.end()); }
    void DefaultButtonModel::addChangeListener(const ::jxx::Ptr<::jxx::swing::event::ChangeListener>& value)
    { if (value) changeListeners_.push_back(value); }
    void DefaultButtonModel::removeChangeListener(const ::jxx::Ptr<::jxx::swing::event::ChangeListener>& value)
    { changeListeners_.erase(std::remove(changeListeners_.begin(), changeListeners_.end(), value), changeListeners_.end()); }

    void DefaultButtonModel::fireStateChanged()
    {
        auto event = ::jxx::NEW<::jxx::swing::event::ChangeEvent>(
            ::jxx::CAST<::jxx::lang::Object>(thisPtr()));
        const auto copy = changeListeners_;
        for (const auto& listener : copy) if (listener) listener->stateChanged(event);
    }
    void DefaultButtonModel::fireActionPerformed()
    {
        auto event = ::jxx::NEW<::jxx::awt::event::ActionEvent>(
            ::jxx::CAST<::jxx::lang::Object>(thisPtr()),
            ::jxx::awt::event::ActionEvent::ACTION_PERFORMED, actionCommand_);
        const auto copy = actionListeners_;
        for (const auto& listener : copy) if (listener) listener->actionPerformed(event);
    }
    void DefaultButtonModel::fireItemStateChanged()
    {
        auto event = ::jxx::NEW<::jxx::awt::event::ItemEvent>(
            ::jxx::CAST<::jxx::awt::ItemSelectable>(thisPtr()),
            ::jxx::awt::event::ItemEvent::ITEM_STATE_CHANGED,
            ::jxx::CAST<::jxx::lang::Object>(thisPtr()),
            selected_ ? ::jxx::awt::event::ItemEvent::SELECTED : ::jxx::awt::event::ItemEvent::DESELECTED);
        const auto copy = itemListeners_;
        for (const auto& listener : copy) if (listener) listener->itemStateChanged(event);
    }

    void DefaultButtonModel::writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out)
    {
        out->writeBoolean(armed_); out->writeBoolean(selected_); out->writeBoolean(enabled_);
        out->writeBoolean(pressed_); out->writeBoolean(rollover_); out->writeInt(mnemonic_);
    }
    void DefaultButtonModel::readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in)
    {
        armed_ = in->readBoolean(); selected_ = in->readBoolean(); enabled_ = in->readBoolean();
        pressed_ = in->readBoolean(); rollover_ = in->readBoolean(); mnemonic_ = in->readInt();
    }
    void DefaultButtonModel::readObjectNoData()
    {
        armed_ = false; selected_ = false; enabled_ = true; pressed_ = false;
        rollover_ = false; mnemonic_ = 0; actionCommand_.reset(); group_.reset();
        actionListeners_.clear(); itemListeners_.clear(); changeListeners_.clear();
    }
}
