#pragma once

#include <cstddef>
#include <vector>

#include "io/jxx.io.InvalidObjectException.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.ClassCastException.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Cloneable.h"
#include "lang/jxx.lang.Enum.h"
#include "lang/jxx.lang.JxxArray.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.Object.h"
#include "util/jxx.util.AbstractMap.h"
#include "util/jxx.util.AbstractSet.h"
#include "util/jxx.util.Iterator.h"
#include "util/jxx.util.Map.h"
#include "util/jxx.util.MapEntry.h"
#include "util/jxx.util.NoSuchElementException.h"

namespace jxx::util {

template <typename K, typename V>
class EnumMap final
    : public ::jxx::lang::ClassBase<
          EnumMap<K, V>,
          AbstractMap<K, V>,
          ::jxx::lang::Cloneable,
          ::jxx::io::SerializableI> {
public:
    using JxxSuper = AbstractMap<K, V>;
    using Super = ::jxx::lang::ClassBase<
        EnumMap<K, V>, JxxSuper, ::jxx::lang::Cloneable,
        ::jxx::io::SerializableI>;
    using JxxClassInfoMarker = typename Super::JxxClassInfoMarker;

private:
    ::jxx::Ptr<::jxx::lang::ClassAny> keyType_;
    std::vector<::jxx::Ptr<V>> values_;
    std::vector<::jxx::lang::jbool> present_;
    ::jxx::lang::jint size_ = 0;
    ::jxx::lang::jint modCount_ = 0;
    ::jxx::Ptr<Set<MapEntry<K, V>>> entrySetView_;

    static auto universe_()
        -> ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<K>, 1U>> {
        return K::values();
    }

    static void requireType_(
        const ::jxx::Ptr<::jxx::lang::ClassAny>& keyType) {
        if (keyType == nullptr) throw ::jxx::lang::NullPointerException();
        if (keyType.get() != K::Class().get()) {
            throw ::jxx::lang::ClassCastException();
        }
    }

    void requireKey_(const ::jxx::Ptr<K>& key) const {
        if (key == nullptr) throw ::jxx::lang::NullPointerException();
        if (key->getDeclaringClass().get() != keyType_.get()) {
            throw ::jxx::lang::ClassCastException();
        }
        const auto ordinal = key->ordinal();
        if (ordinal < 0 || static_cast<std::size_t>(ordinal) >= values_.size()) {
            throw ::jxx::lang::ClassCastException();
        }
    }

    class Entry final
        : public ::jxx::lang::ClassBase<
              Entry, ::jxx::lang::Object, MapEntry<K, V>> {
        ::jxx::Ptr<EnumMap<K, V>> owner_;
        ::jxx::lang::jint ordinal_;

    public:
        Entry(const ::jxx::Ptr<EnumMap<K, V>>& owner,
              ::jxx::lang::jint ordinal)
            : owner_(owner), ordinal_(ordinal) {}

        ::jxx::Ptr<K> getKey() override {
            return (*universe_())[ordinal_];
        }

        ::jxx::Ptr<V> getValue() override {
            return owner_->values_[static_cast<std::size_t>(ordinal_)];
        }

        ::jxx::Ptr<V> setValue(const ::jxx::Ptr<V>& value) override {
            if (!owner_->present_[static_cast<std::size_t>(ordinal_)]) {
                throw ::jxx::lang::IllegalStateException();
            }
            auto previous = owner_->values_[static_cast<std::size_t>(ordinal_)];
            owner_->values_[static_cast<std::size_t>(ordinal_)] = value;
            return previous;
        }

        ::jxx::lang::jbool equals(
            const ::jxx::Ptr<::jxx::lang::Object>& object) const override {
            const auto other = ::jxx::CAST<MapEntry<K, V>>(object);
            if (other == nullptr) return false;
            const auto key = (*universe_())[ordinal_];
            const auto value = owner_->values_[static_cast<std::size_t>(ordinal_)];
            const auto otherKey = other->getKey();
            const auto otherValue = other->getValue();
            return key->equals(::jxx::CAST<::jxx::lang::Object>(otherKey)) &&
                (value == nullptr
                    ? otherValue == nullptr
                    : value->equals(::jxx::CAST<::jxx::lang::Object>(otherValue)));
        }

        ::jxx::lang::jint hashCode() const override {
            const auto key = (*universe_())[ordinal_];
            const auto value = owner_->values_[static_cast<std::size_t>(ordinal_)];
            return key->hashCode() ^ (value == nullptr ? 0 : value->hashCode());
        }
    };

    class EntryIterator final
        : public ::jxx::lang::ClassBase<
              EntryIterator, ::jxx::lang::Object,
              Iterator<MapEntry<K, V>>> {
        ::jxx::Ptr<EnumMap<K, V>> owner_;
        std::size_t next_ = 0U;
        std::size_t last_ = 0U;
        ::jxx::lang::jbool canRemove_ = false;

        void seek_() {
            while (next_ < owner_->present_.size() &&
                   !owner_->present_[next_]) ++next_;
        }

    public:
        explicit EntryIterator(const ::jxx::Ptr<EnumMap<K, V>>& owner)
            : owner_(owner) { seek_(); }

        ::jxx::lang::jbool hasNext() override {
            return next_ < owner_->present_.size();
        }

        ::jxx::Ptr<MapEntry<K, V>> next() override {
            if (!hasNext()) throw NoSuchElementException();
            last_ = next_++;
            canRemove_ = true;
            seek_();
            return ::jxx::CAST<MapEntry<K, V>>(
                ::jxx::NEW<Entry>(owner_,
                    static_cast<::jxx::lang::jint>(last_)));
        }

        void remove() override {
            if (!canRemove_) throw ::jxx::lang::IllegalStateException();
            if (owner_->present_[last_]) {
                owner_->present_[last_] = false;
                owner_->values_[last_].reset();
                --owner_->size_;
                ++owner_->modCount_;
            }
            canRemove_ = false;
        }
    };

    class EntrySet final
        : public ::jxx::lang::ClassBase<
              EntrySet, AbstractSet<MapEntry<K, V>>> {
        ::jxx::Ptr<EnumMap<K, V>> owner_;

    public:
        explicit EntrySet(const ::jxx::Ptr<EnumMap<K, V>>& owner)
            : owner_(owner) {}

        ::jxx::Ptr<Iterator<MapEntry<K, V>>> iterator() override {
            return ::jxx::NEW<EntryIterator>(owner_);
        }

        ::jxx::lang::jint size() override { return owner_->size_; }
        void clear() override { owner_->clear(); }
    };

public:
    explicit EnumMap(
        const ::jxx::Ptr<::jxx::lang::ClassAny>& keyType)
        : keyType_(keyType) {
        requireType_(keyType_);
        const auto universe = universe_();
        const auto count = static_cast<std::size_t>(
            universe == nullptr ? 0 : universe->length);
        values_.resize(count);
        present_.assign(count, false);
    }

    explicit EnumMap(const ::jxx::Ptr<EnumMap<K, V>>& source)
        : keyType_(source == nullptr ? nullptr : source->keyType_) {
        if (source == nullptr) throw ::jxx::lang::NullPointerException();
        values_ = source->values_;
        present_ = source->present_;
        size_ = source->size_;
    }

    explicit EnumMap(const ::jxx::Ptr<Map<K, V>>& source)
        : EnumMap(K::Class()) {
        if (source == nullptr) throw ::jxx::lang::NullPointerException();
        this->putAll(source);
    }

    ::jxx::Ptr<V> put(
        const ::jxx::Ptr<K>& key,
        const ::jxx::Ptr<V>& value) override {
        requireKey_(key);
        const auto index = static_cast<std::size_t>(key->ordinal());
        auto previous = present_[index] ? values_[index] : nullptr;
        values_[index] = value;
        if (!present_[index]) {
            present_[index] = true;
            ++size_;
        }
        ++modCount_;
        return previous;
    }

    ::jxx::Ptr<V> get(
        const ::jxx::Ptr<::jxx::lang::Object>& key) override {
        const auto typed = ::jxx::CAST<K>(key);
        if (typed == nullptr ||
            typed->getDeclaringClass().get() != keyType_.get()) return nullptr;
        const auto index = static_cast<std::size_t>(typed->ordinal());
        return index < present_.size() && present_[index]
            ? values_[index] : nullptr;
    }

    ::jxx::lang::jbool containsKey(
        const ::jxx::Ptr<::jxx::lang::Object>& key) override {
        const auto typed = ::jxx::CAST<K>(key);
        if (typed == nullptr ||
            typed->getDeclaringClass().get() != keyType_.get()) return false;
        const auto index = static_cast<std::size_t>(typed->ordinal());
        return index < present_.size() && present_[index];
    }

    ::jxx::Ptr<V> remove(
        const ::jxx::Ptr<::jxx::lang::Object>& key) override {
        const auto typed = ::jxx::CAST<K>(key);
        if (typed == nullptr ||
            typed->getDeclaringClass().get() != keyType_.get()) return nullptr;
        const auto index = static_cast<std::size_t>(typed->ordinal());
        if (index >= present_.size() || !present_[index]) return nullptr;
        auto previous = values_[index];
        values_[index].reset();
        present_[index] = false;
        --size_;
        ++modCount_;
        return previous;
    }

    ::jxx::lang::jint size() override { return size_; }

    void clear() override {
        if (size_ == 0) return;
        for (std::size_t index = 0U; index < present_.size(); ++index) {
            present_[index] = false;
            values_[index].reset();
        }
        size_ = 0;
        ++modCount_;
    }

    ::jxx::Ptr<Set<MapEntry<K, V>>> entrySet() override {
        if (entrySetView_ == nullptr) {
            entrySetView_ = ::jxx::CAST<Set<MapEntry<K, V>>>(
                ::jxx::NEW<EntrySet>(
                    ::jxx::CAST<EnumMap<K, V>>(this->thisPtr())));
        }
        return entrySetView_;
    }

    ::jxx::Ptr<::jxx::lang::Object> cloneImpl() const override {
        auto result = ::jxx::NEW<EnumMap<K, V>>(keyType_);
        result->values_ = values_;
        result->present_ = present_;
        result->size_ = size_;
        return ::jxx::CAST<::jxx::lang::Object>(result);
    }

    void writeObject(
        const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& output) override {
        if (output == nullptr) throw ::jxx::lang::NullPointerException();
        output->writeInt(size_);
        const auto universe = universe_();
        for (std::size_t index = 0U; index < present_.size(); ++index) {
            if (!present_[index]) continue;
            output->writeObject(::jxx::CAST<::jxx::lang::Object>(
                (*universe)[static_cast<::jxx::lang::jint>(index)]));
            output->writeObject(::jxx::CAST<::jxx::lang::Object>(values_[index]));
        }
    }

    void readObject(
        const ::jxx::Ptr<::jxx::io::ObjectInputStream>& input) override {
        if (input == nullptr) throw ::jxx::lang::NullPointerException();
        keyType_ = K::Class();
        const auto universe = universe_();
        const auto universeSize = static_cast<std::size_t>(
            universe == nullptr ? 0 : universe->length);
        values_.assign(universeSize, nullptr);
        present_.assign(universeSize, false);
        size_ = 0;
        entrySetView_.reset();

        const auto count = input->readInt();
        if (count < 0 || static_cast<std::size_t>(count) > universeSize) {
            throw ::jxx::io::InvalidObjectException("Invalid EnumMap size");
        }
        for (::jxx::lang::jint index = 0; index < count; ++index) {
            const auto key = ::jxx::CAST<K>(input->readObject());
            const auto object = input->readObject();
            const auto value = object == nullptr ? nullptr : ::jxx::CAST<V>(object);
            if (key == nullptr || (object != nullptr && value == nullptr)) {
                throw ::jxx::io::InvalidObjectException(
                    "Invalid EnumMap entry");
            }
            put(key, value);
        }
        modCount_ = 0;
    }

    void readObjectNoData() override {
        keyType_ = K::Class();
        const auto universe = universe_();
        const auto count = static_cast<std::size_t>(
            universe == nullptr ? 0 : universe->length);
        values_.assign(count, nullptr);
        present_.assign(count, false);
        size_ = 0;
        modCount_ = 0;
        entrySetView_.reset();
    }
};

} // namespace jxx::util
