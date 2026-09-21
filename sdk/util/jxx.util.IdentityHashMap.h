#pragma once
#include <cstdint>

#include <vector>

#include "util/jxx.util.HashMap.h"
#include "util/jxx.util.HashSet.h"

namespace jxx::util {

template <typename K, typename V>
class IdentityHashMap final : public HashMap<K, V> {
public:
    using JxxSuper = HashMap<K, V>;

    IdentityHashMap() = default;
    explicit IdentityHashMap(::jxx::lang::jint expectedMaximumSize) {
        if (expectedMaximumSize < 0) throw ::jxx::lang::IllegalArgumentException();
        entries_.reserve(static_cast<std::size_t>(expectedMaximumSize));
    }
    explicit IdentityHashMap(const ::jxx::Ptr<Map<K, V>>& source) {
        if (source == nullptr) throw ::jxx::lang::NullPointerException();
        auto iterator = source->entrySet()->iterator();
        while (iterator->hasNext()) {
            const auto entry = iterator->next();
            put(entry->getKey(), entry->getValue());
        }
    }

    ::jxx::lang::jint size() override {
        return static_cast<::jxx::lang::jint>(entries_.size());
    }
    ::jxx::lang::jbool isEmpty() override { return entries_.empty(); }
    ::jxx::lang::jbool containsKey(
        const ::jxx::Ptr<::jxx::lang::Object>& key) override {
        return findKey_(key) >= 0;
    }
    ::jxx::lang::jbool containsValue(
        const ::jxx::Ptr<::jxx::lang::Object>& value) override {
        for (const auto& entry : entries_)
            if (entry.value.get() == value.get()) return true;
        return false;
    }
    ::jxx::Ptr<V> get(
        const ::jxx::Ptr<::jxx::lang::Object>& key) override {
        const auto index = findKey_(key);
        return index < 0 ? nullptr : entries_[static_cast<std::size_t>(index)].value;
    }
    ::jxx::Ptr<V> put(
        const ::jxx::Ptr<K>& key,
        const ::jxx::Ptr<V>& value) override {
        const auto object = ::jxx::CAST<::jxx::lang::Object>(key);
        const auto index = findKey_(object);
        if (index >= 0) {
            auto& stored = entries_[static_cast<std::size_t>(index)].value;
            auto previous = stored; stored = value; return previous;
        }
        entries_.push_back({key, value});
        return nullptr;
    }
    ::jxx::Ptr<V> remove(
        const ::jxx::Ptr<::jxx::lang::Object>& key) override {
        const auto index = findKey_(key);
        if (index < 0) return nullptr;
        const auto position = entries_.begin() + index;
        auto previous = position->value;
        entries_.erase(position);
        return previous;
    }
    void clear() override { entries_.clear(); }

    ::jxx::Ptr<Set<MapEntry<K, V>>> entrySet() override {
        auto result = ::jxx::NEW<HashSet<MapEntry<K, V>>>();
        for (const auto& entry : entries_)
            result->add(::jxx::CAST<MapEntry<K, V>>(
                ::jxx::NEW<EntrySnapshot>(entry.key, entry.value)));
        return ::jxx::CAST<Set<MapEntry<K, V>>>(result);
    }

private:
    class EntrySnapshot final : public ::jxx::lang::ClassBase<
        EntrySnapshot, ::jxx::lang::Object, MapEntry<K, V>> {
    public:
        EntrySnapshot(const ::jxx::Ptr<K>& key, const ::jxx::Ptr<V>& value)
            : key_(key), value_(value) {}
        ::jxx::Ptr<K> getKey() override { return key_; }
        ::jxx::Ptr<V> getValue() override { return value_; }
        ::jxx::Ptr<V> setValue(const ::jxx::Ptr<V>& value) override {
            auto old=value_; value_=value; return old;
        }
        ::jxx::lang::jbool equals(const ::jxx::Ptr<::jxx::lang::Object>& object) const override {
            auto other=::jxx::CAST<MapEntry<K,V>>(object);
            return other != nullptr && key_.get()==other->getKey().get() &&
                value_.get()==other->getValue().get();
        }
        ::jxx::lang::jint hashCode() const override {
            const auto keyHash=static_cast<::jxx::lang::jint>(reinterpret_cast<std::uintptr_t>(key_.get()));
            const auto valueHash=static_cast<::jxx::lang::jint>(reinterpret_cast<std::uintptr_t>(value_.get()));
            return keyHash ^ valueHash;
        }
    private: ::jxx::Ptr<K> key_; ::jxx::Ptr<V> value_;
    };
    struct IdentityEntry { ::jxx::Ptr<K> key; ::jxx::Ptr<V> value; };
    ::jxx::lang::jint findKey_(
        const ::jxx::Ptr<::jxx::lang::Object>& key) const {
        for (std::size_t index = 0; index < entries_.size(); ++index) {
            if (entries_[index].key.get() == key.get())
                return static_cast<::jxx::lang::jint>(index);
        }
        return -1;
    }
    std::vector<IdentityEntry> entries_;
};

} // namespace jxx::util
