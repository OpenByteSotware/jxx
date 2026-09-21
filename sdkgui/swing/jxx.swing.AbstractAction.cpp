#include "swing/jxx.swing.AbstractAction.h"

#include <algorithm>

#include "beans/jxx.beans.PropertyChangeEvent.h"
#include "lang/jxx.lang.Boolean.h"
#include "swing/jxx.swing.Icon.h"

namespace jxx::swing
{
    AbstractAction::AbstractAction() : Super()
    {
    }

    AbstractAction::AbstractAction(
        const ::jxx::Ptr<::jxx::lang::String>& name)
        : Super()
    {
        putValue(Action::NAME, name);
    }

    AbstractAction::AbstractAction(
        const ::jxx::Ptr<::jxx::lang::String>& name,
        const ::jxx::Ptr<Icon>& icon)
        : Super()
    {
        putValue(Action::NAME, name);
        putValue(Action::SMALL_ICON,
            ::jxx::CAST<::jxx::lang::Object>(icon));
    }

    ::jxx::lang::jbool AbstractAction::sameKey(
        const ::jxx::Ptr<::jxx::lang::String>& left,
        const ::jxx::Ptr<::jxx::lang::String>& right)
    {
        return left == nullptr ? right == nullptr
            : left->equals(::jxx::CAST<::jxx::lang::Object>(right));
    }

    ::jxx::Ptr<::jxx::lang::Object> AbstractAction::getValue(
        const ::jxx::Ptr<::jxx::lang::String>& key) const
    {
        for (const auto& entry : values_)
            if (sameKey(entry.key, key)) return entry.value;
        return nullptr;
    }

    void AbstractAction::putValue(
        const ::jxx::Ptr<::jxx::lang::String>& key,
        const ::jxx::Ptr<::jxx::lang::Object>& value)
    {
        if (key == nullptr) return;
        for (auto iterator = values_.begin(); iterator != values_.end();
             ++iterator)
        {
            if (!sameKey(iterator->key, key)) continue;
            const auto oldValue = iterator->value;
            if (value == nullptr) values_.erase(iterator);
            else iterator->value = value;
            firePropertyChange(key, oldValue, value);
            return;
        }
        if (value != nullptr) values_.push_back({key, value});
        firePropertyChange(key, nullptr, value);
    }

    void AbstractAction::setEnabled(::jxx::lang::jbool enabled)
    {
        if (enabled_ == enabled) return;
        const auto oldValue = ::jxx::CAST<::jxx::lang::Object>(
            ::jxx::NEW<::jxx::lang::Boolean>(enabled_));
        const auto newValue = ::jxx::CAST<::jxx::lang::Object>(
            ::jxx::NEW<::jxx::lang::Boolean>(enabled));
        enabled_ = enabled;
        firePropertyChange(
            ::jxx::NEW<::jxx::lang::String>("enabled"),
            oldValue, newValue);
    }

    ::jxx::lang::jbool AbstractAction::isEnabled() const
    {
        return enabled_;
    }

    void AbstractAction::addPropertyChangeListener(
        const ::jxx::Ptr<::jxx::beans::PropertyChangeListener>& listener)
    {
        if (listener != nullptr) listeners_.push_back(listener);
    }

    void AbstractAction::removePropertyChangeListener(
        const ::jxx::Ptr<::jxx::beans::PropertyChangeListener>& listener)
    {
        listeners_.erase(
            std::remove(listeners_.begin(), listeners_.end(), listener),
            listeners_.end());
    }

    void AbstractAction::firePropertyChange(
        const ::jxx::Ptr<::jxx::lang::String>& propertyName,
        const ::jxx::Ptr<::jxx::lang::Object>& oldValue,
        const ::jxx::Ptr<::jxx::lang::Object>& newValue)
    {
        auto event = ::jxx::NEW<::jxx::beans::PropertyChangeEvent>(
            ::jxx::CAST<::jxx::lang::Object>(thisPtr()),
            propertyName, oldValue, newValue);
        const auto listeners = listeners_;
        for (const auto& listener : listeners)
            if (listener != nullptr) listener->propertyChange(event);
    }

    void AbstractAction::writeObject(
        const ::jxx::Ptr<::jxx::io::ObjectOutputStream>&)
    {
    }

    void AbstractAction::readObject(
        const ::jxx::Ptr<::jxx::io::ObjectInputStream>&)
    {
        values_.clear();
        listeners_.clear();
        enabled_ = true;
    }

    void AbstractAction::readObjectNoData()
    {
        values_.clear();
        listeners_.clear();
        enabled_ = true;
    }
}
