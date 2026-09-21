#include "swing/jxx.swing.UIDefaults.h"

#include <algorithm>

#include "awt/jxx.awt.Color.h"
#include "awt/jxx.awt.Dimension.h"
#include "awt/jxx.awt.Font.h"
#include "awt/jxx.awt.Insets.h"
#include "lang/jxx.lang.Boolean.h"
#include "lang/jxx.lang.Integer.h"
#include "swing/jxx.swing.Icon.h"

namespace jxx::swing
{
    UIDefaults::UIDefaults() : Super()
    {
    }

    ::jxx::lang::jbool UIDefaults::sameKey(
        const ::jxx::Ptr<::jxx::lang::Object>& left,
        const ::jxx::Ptr<::jxx::lang::Object>& right)
    {
        return left == nullptr ? right == nullptr : left->equals(right);
    }

    ::jxx::Ptr<::jxx::lang::Object> UIDefaults::get(
        const ::jxx::Ptr<::jxx::lang::Object>& key) const
    {
        for (const auto& entry : entries_)
            if (sameKey(entry.key, key)) return entry.value;
        return nullptr;
    }

    ::jxx::Ptr<::jxx::lang::Object> UIDefaults::put(
        const ::jxx::Ptr<::jxx::lang::Object>& key,
        const ::jxx::Ptr<::jxx::lang::Object>& value)
    {
        if (key == nullptr) return nullptr;
        for (auto iterator = entries_.begin(); iterator != entries_.end();
             ++iterator)
        {
            if (!sameKey(iterator->key, key)) continue;
            const auto oldValue = iterator->value;
            if (value == nullptr) entries_.erase(iterator);
            else iterator->value = value;
            return oldValue;
        }
        if (value != nullptr) entries_.push_back({key, value});
        return nullptr;
    }

    ::jxx::Ptr<::jxx::lang::Object> UIDefaults::remove(
        const ::jxx::Ptr<::jxx::lang::Object>& key)
    {
        return put(key, nullptr);
    }

    void UIDefaults::clear()
    {
        entries_.clear();
    }

    ::jxx::lang::jint UIDefaults::size() const
    {
        return static_cast<::jxx::lang::jint>(entries_.size());
    }

    ::jxx::lang::jbool UIDefaults::containsKey(
        const ::jxx::Ptr<::jxx::lang::Object>& key) const
    {
        return get(key) != nullptr;
    }

    ::jxx::Ptr<::jxx::lang::String> UIDefaults::getString(
        const ::jxx::Ptr<::jxx::lang::Object>& key) const
    { return ::jxx::CAST<::jxx::lang::String>(get(key)); }

    ::jxx::Ptr<::jxx::awt::Color> UIDefaults::getColor(
        const ::jxx::Ptr<::jxx::lang::Object>& key) const
    { return ::jxx::CAST<::jxx::awt::Color>(get(key)); }

    ::jxx::Ptr<::jxx::awt::Font> UIDefaults::getFont(
        const ::jxx::Ptr<::jxx::lang::Object>& key) const
    { return ::jxx::CAST<::jxx::awt::Font>(get(key)); }

    ::jxx::Ptr<Icon> UIDefaults::getIcon(
        const ::jxx::Ptr<::jxx::lang::Object>& key) const
    { return ::jxx::CAST<Icon>(get(key)); }

    ::jxx::Ptr<::jxx::awt::Insets> UIDefaults::getInsets(
        const ::jxx::Ptr<::jxx::lang::Object>& key) const
    { return ::jxx::CAST<::jxx::awt::Insets>(get(key)); }

    ::jxx::Ptr<::jxx::awt::Dimension> UIDefaults::getDimension(
        const ::jxx::Ptr<::jxx::lang::Object>& key) const
    { return ::jxx::CAST<::jxx::awt::Dimension>(get(key)); }

    ::jxx::lang::jint UIDefaults::getInt(
        const ::jxx::Ptr<::jxx::lang::Object>& key) const
    {
        const auto value = ::jxx::CAST<::jxx::lang::Integer>(get(key));
        return value == nullptr ? 0 : value->intValue();
    }

    ::jxx::lang::jbool UIDefaults::getBoolean(
        const ::jxx::Ptr<::jxx::lang::Object>& key) const
    {
        const auto value = ::jxx::CAST<::jxx::lang::Boolean>(get(key));
        return value != nullptr && value->booleanValue();
    }

    void UIDefaults::writeObject(
        const ::jxx::Ptr<::jxx::io::ObjectOutputStream>&)
    {
    }

    void UIDefaults::readObject(
        const ::jxx::Ptr<::jxx::io::ObjectInputStream>&)
    {
        entries_.clear();
    }

    void UIDefaults::readObjectNoData()
    {
        entries_.clear();
    }
}
