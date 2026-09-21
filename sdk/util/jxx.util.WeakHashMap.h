#pragma once
#include <cstdint>

#include <algorithm>
#include <cmath>
#include <memory>
#include <vector>

#include "util/jxx.util.HashMap.h"
#include "util/jxx.util.HashSet.h"

namespace jxx::util {

template <typename K, typename V>
class WeakHashMap final : public HashMap<K, V> {
public:
    using JxxSuper = HashMap<K, V>;

    WeakHashMap() = default;
    explicit WeakHashMap(::jxx::lang::jint initialCapacity)
        : WeakHashMap(initialCapacity, 0.75F) {}
    WeakHashMap(::jxx::lang::jint initialCapacity,
                ::jxx::lang::jfloat loadFactor) {
        if (initialCapacity < 0 || !(loadFactor > 0.0F) ||
            std::isnan(static_cast<double>(loadFactor)))
            throw ::jxx::lang::IllegalArgumentException();
        entries_.reserve(static_cast<std::size_t>(initialCapacity));
    }
    explicit WeakHashMap(const ::jxx::Ptr<Map<K, V>>& source) {
        if (source == nullptr) throw ::jxx::lang::NullPointerException();
        auto iterator = source->entrySet()->iterator();
        while (iterator->hasNext()) {
            const auto entry = iterator->next();
            put(entry->getKey(), entry->getValue());
        }
    }

    ::jxx::lang::jint size() override {
        expunge_();
        return static_cast<::jxx::lang::jint>(entries_.size()) + (hasNullKey_ ? 1 : 0);
    }
    ::jxx::lang::jbool isEmpty() override { return size() == 0; }
    ::jxx::lang::jbool containsKey(
        const ::jxx::Ptr<::jxx::lang::Object>& key) override {
        expunge_();
        if (key == nullptr) return hasNullKey_;
        return findKey_(key) >= 0;
    }
    ::jxx::lang::jbool containsValue(
        const ::jxx::Ptr<::jxx::lang::Object>& value) override {
        expunge_();
        if (hasNullKey_ && equalObject_(nullValue_, value)) return true;
        for (const auto& entry : entries_)
            if (equalObject_(entry.value, value)) return true;
        return false;
    }
    ::jxx::Ptr<V> get(
        const ::jxx::Ptr<::jxx::lang::Object>& key) override {
        expunge_();
        if (key == nullptr) return hasNullKey_ ? nullValue_ : nullptr;
        const auto index = findKey_(key);
        return index < 0 ? nullptr : entries_[static_cast<std::size_t>(index)].value;
    }
    ::jxx::Ptr<V> put(
        const ::jxx::Ptr<K>& key,
        const ::jxx::Ptr<V>& value) override {
        expunge_();
        if (key == nullptr) {
            auto previous = hasNullKey_ ? nullValue_ : nullptr;
            hasNullKey_ = true; nullValue_ = value; return previous;
        }
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
        expunge_();
        if (key == nullptr) {
            if (!hasNullKey_) return nullptr;
            hasNullKey_ = false; auto previous = nullValue_; nullValue_.reset(); return previous;
        }
        const auto index = findKey_(key);
        if (index < 0) return nullptr;
        const auto position = entries_.begin() + index;
        auto previous = position->value; entries_.erase(position); return previous;
    }
    void clear() override {
        entries_.clear(); hasNullKey_ = false; nullValue_.reset();
    }

    ::jxx::Ptr<Set<MapEntry<K, V>>> entrySet() override {
        expunge_();
        auto result = ::jxx::NEW<HashSet<MapEntry<K, V>>>();
        if (hasNullKey_)
            result->add(::jxx::CAST<MapEntry<K, V>>(
                ::jxx::NEW<EntrySnapshot>(nullptr, nullValue_)));
        for (const auto& entry : entries_) {
            auto key=entry.key.lock();
            if (key != nullptr)
                result->add(::jxx::CAST<MapEntry<K, V>>(
                    ::jxx::NEW<EntrySnapshot>(key, entry.value)));
        }
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
        ::jxx::Ptr<V> setValue(const ::jxx::Ptr<V>& value) override {auto old=value_;value_=value;return old;}
        ::jxx::lang::jbool equals(const ::jxx::Ptr<::jxx::lang::Object>& object) const override {
            auto other=::jxx::CAST<MapEntry<K,V>>(object); if(other==nullptr)return false;
            const auto ok=other->getKey(); const auto ov=other->getValue();
            const auto keyEqual=key_==nullptr?ok==nullptr:key_->equals(::jxx::CAST<::jxx::lang::Object>(ok));
            const auto valueEqual=value_==nullptr?ov==nullptr:value_->equals(::jxx::CAST<::jxx::lang::Object>(ov));
            return keyEqual&&valueEqual;
        }
        ::jxx::lang::jint hashCode() const override {
            return (key_?key_->hashCode():0)^(value_?value_->hashCode():0);
        }
    private: ::jxx::Ptr<K> key_; ::jxx::Ptr<V> value_;
    };
    struct WeakEntry { std::weak_ptr<K> key; ::jxx::Ptr<V> value; };
    static ::jxx::lang::jbool equalObject_(
        const ::jxx::Ptr<V>& value,
        const ::jxx::Ptr<::jxx::lang::Object>& object) {
        if (value == nullptr || object == nullptr) return value == nullptr && object == nullptr;
        const auto valueObject = ::jxx::CAST<::jxx::lang::Object>(value);
        return valueObject != nullptr && valueObject->equals(object);
    }
    void expunge_() {
        entries_.erase(
            std::remove_if(entries_.begin(), entries_.end(),
                [](const WeakEntry& entry) { return entry.key.expired(); }),
            entries_.end());
    }
    ::jxx::lang::jint findKey_(
        const ::jxx::Ptr<::jxx::lang::Object>& key) const {
        for (std::size_t index = 0; index < entries_.size(); ++index) {
            const auto candidate = entries_[index].key.lock();
            if (candidate == nullptr) continue;
            const auto candidateObject = ::jxx::CAST<::jxx::lang::Object>(candidate);
            if (candidateObject == key ||
                (candidateObject != nullptr && candidateObject->equals(key)))
                return static_cast<::jxx::lang::jint>(index);
        }
        return -1;
    }
    std::vector<WeakEntry> entries_;
    ::jxx::lang::jbool hasNullKey_ = false;
    ::jxx::Ptr<V> nullValue_;
};

} // namespace jxx::util
