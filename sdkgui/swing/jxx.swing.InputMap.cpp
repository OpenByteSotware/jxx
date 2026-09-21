#include "swing/jxx.swing.InputMap.h"

#include <algorithm>

#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "swing/jxx.swing.KeyStroke.h"

namespace jxx::swing
{
    InputMap::InputMap() : Super()
    {
    }

    void InputMap::setParent(const ::jxx::Ptr<InputMap>& map)
    {
        parent_ = map;
    }

    ::jxx::Ptr<InputMap> InputMap::getParent() const
    {
        return parent_;
    }

    ::jxx::lang::jbool InputMap::sameKey(
        const ::jxx::Ptr<KeyStroke>& left,
        const ::jxx::Ptr<KeyStroke>& right)
    {
        if (left == nullptr || right == nullptr) return left == right;
        return left->getKeyCode() == right->getKeyCode()
            && left->getKeyChar() == right->getKeyChar()
            && left->getModifiers() == right->getModifiers()
            && left->isOnKeyRelease() == right->isOnKeyRelease();
    }

    void InputMap::put(
        const ::jxx::Ptr<KeyStroke>& keyStroke,
        const ::jxx::Ptr<::jxx::lang::Object>& actionMapKey)
    {
        if (keyStroke == nullptr) return;
        if (actionMapKey == nullptr)
        {
            remove(keyStroke);
            return;
        }

        for (auto& entry : entries_)
        {
            if (sameKey(entry.key, keyStroke))
            {
                entry.value = actionMapKey;
                return;
            }
        }
        entries_.push_back({keyStroke, actionMapKey});
    }

    ::jxx::Ptr<::jxx::lang::Object> InputMap::get(
        const ::jxx::Ptr<KeyStroke>& keyStroke) const
    {
        if (keyStroke == nullptr) return nullptr;
        for (const auto& entry : entries_)
            if (sameKey(entry.key, keyStroke)) return entry.value;
        return parent_ == nullptr ? nullptr : parent_->get(keyStroke);
    }

    void InputMap::remove(const ::jxx::Ptr<KeyStroke>& keyStroke)
    {
        entries_.erase(
            std::remove_if(entries_.begin(), entries_.end(),
                [&keyStroke](const Entry& entry)
                { return sameKey(entry.key, keyStroke); }),
            entries_.end());
    }

    void InputMap::clear()
    {
        entries_.clear();
    }

    InputMap::KeyStrokeArray InputMap::keys() const
    {
        if (entries_.empty()) return nullptr;
        auto result = ::jxx::NEW<::jxx::lang::JxxArray<
            ::jxx::Ptr<KeyStroke>, 1>>(
                static_cast<std::uint32_t>(entries_.size()));
        for (std::uint32_t index = 0; index < result->length; ++index)
            (*result)[index] = entries_[index].key;
        return result;
    }

    InputMap::KeyStrokeArray InputMap::allKeys() const
    {
        std::vector<::jxx::Ptr<KeyStroke>> values;
        values.reserve(entries_.size());
        for (const auto& entry : entries_) values.push_back(entry.key);

        const auto parentKeys = parent_ == nullptr ? nullptr : parent_->allKeys();
        if (parentKeys != nullptr)
        {
            for (std::uint32_t index = 0; index < parentKeys->length; ++index)
            {
                const auto candidate = (*parentKeys)[index];
                const auto found = std::find_if(values.begin(), values.end(),
                    [&candidate](const ::jxx::Ptr<KeyStroke>& value)
                    { return sameKey(value, candidate); });
                if (found == values.end()) values.push_back(candidate);
            }
        }

        if (values.empty()) return nullptr;
        auto result = ::jxx::NEW<::jxx::lang::JxxArray<
            ::jxx::Ptr<KeyStroke>, 1>>(
                static_cast<std::uint32_t>(values.size()));
        for (std::uint32_t index = 0; index < result->length; ++index)
            (*result)[index] = values[index];
        return result;
    }

    ::jxx::lang::jint InputMap::size() const
    {
        return static_cast<::jxx::lang::jint>(entries_.size());
    }

    const std::vector<InputMap::Entry>& InputMap::entries() const noexcept
    {
        return entries_;
    }

    void InputMap::writeObject(
        const ::jxx::Ptr<::jxx::io::ObjectOutputStream>&)
    {
    }

    void InputMap::readObject(
        const ::jxx::Ptr<::jxx::io::ObjectInputStream>&)
    {
        entries_.clear();
        parent_.reset();
    }

    void InputMap::readObjectNoData()
    {
        entries_.clear();
        parent_.reset();
    }
}
