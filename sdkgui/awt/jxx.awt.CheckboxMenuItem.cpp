#include "awt/jxx.awt.CheckboxMenuItem.h"

#include <algorithm>

#include "awt/event/jxx.awt.event.ItemEvent.h"
#include "awt/event/jxx.awt.event.ItemListener.h"

namespace jxx::awt
{
    CheckboxMenuItem::CheckboxMenuItem()
        : CheckboxMenuItem(::jxx::NEW<::jxx::lang::String>(""), false)
    {
    }

    CheckboxMenuItem::CheckboxMenuItem(
        const ::jxx::Ptr<::jxx::lang::String>& label)
        : CheckboxMenuItem(label, false)
    {
    }

    CheckboxMenuItem::CheckboxMenuItem(
        const ::jxx::Ptr<::jxx::lang::String>& label,
        ::jxx::lang::jbool state)
        : Super(label), state_(state)
    {
    }

    ::jxx::lang::jbool CheckboxMenuItem::getState() const
    {
        return state_;
    }

    void CheckboxMenuItem::setState(::jxx::lang::jbool state)
    {
        state_ = state;
    }

    void CheckboxMenuItem::addItemListener(
        const ::jxx::Ptr<::jxx::awt::event::ItemListener>& listener)
    {
        if (listener != nullptr &&
            std::find(listeners_.begin(), listeners_.end(), listener) ==
                listeners_.end())
            listeners_.push_back(listener);
    }

    void CheckboxMenuItem::removeItemListener(
        const ::jxx::Ptr<::jxx::awt::event::ItemListener>& listener)
    {
        listeners_.erase(
            std::remove(listeners_.begin(), listeners_.end(), listener),
            listeners_.end());
    }

    void CheckboxMenuItem::setStateFromNative(::jxx::lang::jbool state)
    {
        if (state_ == state) return;
        state_ = state;
        processItemEvent(::jxx::NEW<::jxx::awt::event::ItemEvent>(
            ::jxx::CAST<::jxx::lang::Object>(thisPtr()),
            ::jxx::awt::event::ItemEvent::ITEM_STATE_CHANGED,
            ::jxx::CAST<::jxx::lang::Object>(getLabel()),
            state ? ::jxx::awt::event::ItemEvent::SELECTED
                  : ::jxx::awt::event::ItemEvent::DESELECTED));
    }

    void CheckboxMenuItem::processItemEvent(
        const ::jxx::Ptr<::jxx::awt::event::ItemEvent>& event)
    {
        const auto listeners = listeners_;
        for (const auto& listener : listeners)
            if (listener != nullptr) listener->itemStateChanged(event);
    }
}
