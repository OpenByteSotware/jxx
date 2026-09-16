#include "awt/jxx.awt.Checkbox.h"

#include <algorithm>

#include "awt/event/jxx.awt.event.ItemEvent.h"
#include "awt/event/jxx.awt.event.ItemListener.h"
#include "awt/jxx.awt.CheckboxGroup.h"
#include "gui/internal/jxx.gui.internal.NativeComponent.h"

namespace jxx::awt
{
    namespace
    {
        ::jxx::Ptr<::jxx::lang::String> emptyText()
        {
            return ::jxx::NEW<::jxx::lang::String>("");
        }
    }

    Checkbox::Checkbox() : Checkbox(emptyText(), false, nullptr) {}

    Checkbox::Checkbox(const ::jxx::Ptr<::jxx::lang::String>& label)
        : Checkbox(label, false, nullptr) {}

    Checkbox::Checkbox(const ::jxx::Ptr<::jxx::lang::String>& label,
        ::jxx::lang::jbool state)
        : Checkbox(label, state, nullptr) {}

    Checkbox::Checkbox(const ::jxx::Ptr<::jxx::lang::String>& label,
        ::jxx::lang::jbool state,
        const ::jxx::Ptr<CheckboxGroup>& group)
        : label_(label != nullptr ? label : emptyText()),
          state_(false),
          group_(group)
    {
        setState(state);
    }

    Checkbox::Checkbox(const ::jxx::Ptr<::jxx::lang::String>& label,
        const ::jxx::Ptr<CheckboxGroup>& group,
        ::jxx::lang::jbool state)
        : Checkbox(label, state, group) {}

    ::jxx::Ptr<::jxx::lang::String> Checkbox::getLabel() const { return label_; }
    void Checkbox::setLabel(const ::jxx::Ptr<::jxx::lang::String>& label)
    {
        label_ = label != nullptr ? label : emptyText();
        if (nativeComponent_ != nullptr) nativeComponent_->setText(label_);
        invalidate();
    }

    ::jxx::lang::jbool Checkbox::getState() const { return state_; }

    void Checkbox::setState(::jxx::lang::jbool state)
    {
        if (state && group_ != nullptr)
        {
            group_->setSelectedCheckbox(::jxx::CAST<Checkbox>(thisPtr()));
            return;
        }
        if (!state && group_ != nullptr &&
            group_->getSelectedCheckbox().get() == this)
            return;
        state_ = state;
    }

    ::jxx::Ptr<CheckboxGroup> Checkbox::getCheckboxGroup() const { return group_; }

    void Checkbox::setCheckboxGroup(const ::jxx::Ptr<CheckboxGroup>& group)
    {
        if (group_ == group) return;
        const auto oldGroup = group_;
        group_ = group;
        if (oldGroup != nullptr && oldGroup->getSelectedCheckbox().get() == this)
            oldGroup->setSelectedCheckbox(nullptr);
        if (state_ && group_ != nullptr)
            group_->setSelectedCheckbox(::jxx::CAST<Checkbox>(thisPtr()));
    }

    ::jxx::lang::ObjectArray Checkbox::getSelectedObjects() const
    {
        if (!state_) return nullptr;
        auto result = ::jxx::NEW<::jxx::lang::ObjectArrayType>(1);
        (*result)[0] = ::jxx::CAST<::jxx::lang::Object>(label_);
        return result;
    }

    void Checkbox::addItemListener(
        const ::jxx::Ptr<::jxx::awt::event::ItemListener>& listener)
    {
        if (listener != nullptr &&
            std::find(listeners_.begin(), listeners_.end(), listener) == listeners_.end())
            listeners_.push_back(listener);
    }

    void Checkbox::removeItemListener(
        const ::jxx::Ptr<::jxx::awt::event::ItemListener>& listener)
    {
        listeners_.erase(std::remove(listeners_.begin(), listeners_.end(), listener),
            listeners_.end());
    }

    void Checkbox::setStateFromGroup(::jxx::lang::jbool state) { state_ = state; }

    void Checkbox::fireItemEvent(::jxx::lang::jint stateChange)
    {
        processItemEvent(::jxx::NEW<::jxx::awt::event::ItemEvent>(
            ::jxx::CAST<ItemSelectable>(thisPtr()),
            ::jxx::awt::event::ItemEvent::ITEM_STATE_CHANGED,
            ::jxx::CAST<::jxx::lang::Object>(label_), stateChange));
    }

    void Checkbox::processItemEvent(
        const ::jxx::Ptr<::jxx::awt::event::ItemEvent>& event)
    {
        const auto listeners = listeners_;
        for (const auto& listener : listeners)
            if (listener != nullptr) listener->itemStateChanged(event);
    }
}
