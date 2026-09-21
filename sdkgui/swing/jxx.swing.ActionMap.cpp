#include "swing/jxx.swing.ActionMap.h"

#include <algorithm>

#include "swing/jxx.swing.Action.h"

namespace jxx::swing
{
    ActionMap::ActionMap() : Super()
    {
    }

    ::jxx::lang::jbool ActionMap::sameKey(
        const ::jxx::Ptr<::jxx::lang::Object>& left,
        const ::jxx::Ptr<::jxx::lang::Object>& right)
    {
        return left == nullptr ? right == nullptr : left->equals(right);
    }

    void ActionMap::setParent(const ::jxx::Ptr<ActionMap>& map)
    {
        parent_ = map;
    }

    ::jxx::Ptr<ActionMap> ActionMap::getParent() const
    {
        return parent_;
    }

    void ActionMap::put(
        const ::jxx::Ptr<::jxx::lang::Object>& key,
        const ::jxx::Ptr<Action>& action)
    {
        if (key == nullptr) return;
        for (auto iterator = values_.begin(); iterator != values_.end();
             ++iterator)
        {
            if (!sameKey(iterator->key, key)) continue;
            if (action == nullptr) values_.erase(iterator);
            else iterator->action = action;
            return;
        }
        if (action != nullptr) values_.push_back({key, action});
    }

    ::jxx::Ptr<Action> ActionMap::get(
        const ::jxx::Ptr<::jxx::lang::Object>& key) const
    {
        if (key == nullptr) return nullptr;
        for (const auto& entry : values_)
            if (sameKey(entry.key, key)) return entry.action;
        return parent_ == nullptr ? nullptr : parent_->get(key);
    }

    void ActionMap::remove(
        const ::jxx::Ptr<::jxx::lang::Object>& key)
    {
        values_.erase(
            std::remove_if(values_.begin(), values_.end(),
                [&key](const Entry& entry)
                { return sameKey(entry.key, key); }),
            values_.end());
    }

    void ActionMap::clear()
    {
        values_.clear();
    }

    ActionMap::KeyArray ActionMap::keys() const
    {
        if (values_.empty()) return nullptr;
        auto result = ::jxx::NEW<::jxx::lang::JxxArray<
            ::jxx::Ptr<::jxx::lang::Object>, 1>>(
                static_cast<std::uint32_t>(values_.size()));
        for (std::uint32_t index = 0; index < result->length; ++index)
            (*result)[index] = values_[index].key;
        return result;
    }

    ActionMap::KeyArray ActionMap::allKeys() const
    {
        std::vector<::jxx::Ptr<::jxx::lang::Object>> keys;
        for (const auto& entry : values_) keys.push_back(entry.key);
        const auto inherited = parent_ == nullptr ? nullptr : parent_->allKeys();
        if (inherited != nullptr)
        {
            for (std::uint32_t index = 0; index < inherited->length; ++index)
            {
                const auto candidate = (*inherited)[index];
                const auto found = std::find_if(keys.begin(), keys.end(),
                    [&candidate](const auto& key)
                    { return sameKey(key, candidate); });
                if (found == keys.end()) keys.push_back(candidate);
            }
        }
        if (keys.empty()) return nullptr;
        auto result = ::jxx::NEW<::jxx::lang::JxxArray<
            ::jxx::Ptr<::jxx::lang::Object>, 1>>(
                static_cast<std::uint32_t>(keys.size()));
        for (std::uint32_t index = 0; index < result->length; ++index)
            (*result)[index] = keys[index];
        return result;
    }

    ::jxx::lang::jint ActionMap::size() const
    {
        return static_cast<::jxx::lang::jint>(values_.size());
    }

    void ActionMap::writeObject(
        const ::jxx::Ptr<::jxx::io::ObjectOutputStream>&)
    {
    }

    void ActionMap::readObject(
        const ::jxx::Ptr<::jxx::io::ObjectInputStream>&)
    {
        values_.clear();
        parent_.reset();
    }

    void ActionMap::readObjectNoData()
    {
        values_.clear();
        parent_.reset();
    }
}
