#pragma once

#include <cstdint>
#include <vector>

#include "io/jxx.io.InvalidObjectException.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.ClassCastException.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Cloneable.h"
#include "lang/jxx.lang.Enum.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.JxxArray.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.Object.h"
#include "util/jxx.util.AbstractSet.h"
#include "util/jxx.util.Collection.h"
#include "util/jxx.util.Iterator.h"
#include "util/jxx.util.NoSuchElementException.h"

namespace jxx::util {

template <typename E>
class EnumSet final
    : public ::jxx::lang::ClassBase<
          EnumSet<E>,
          AbstractSet<E>,
          ::jxx::lang::Cloneable,
          ::jxx::io::SerializableI> {
public:
    using JxxSuper = AbstractSet<E>;
    using Super = ::jxx::lang::ClassBase<
        EnumSet<E>, JxxSuper, ::jxx::lang::Cloneable, ::jxx::io::SerializableI>;
    using JxxClassInfoMarker = typename Super::JxxClassInfoMarker;

private:
    struct ConstructionToken final {};

    ::jxx::Ptr<::jxx::lang::ClassAny> elementType_;
    std::vector<::jxx::lang::jbool> present_;
    ::jxx::lang::jint size_ = 0;
    ::jxx::lang::jint modCount_ = 0;

    static ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<E>, 1U>> universe_() {
        return E::values();
    }

    static void requireType_(
        const ::jxx::Ptr<::jxx::lang::ClassAny>& elementType) {
        if (elementType == nullptr) throw ::jxx::lang::NullPointerException();
        if (elementType.get() != E::Class().get()) {
            throw ::jxx::lang::ClassCastException();
        }
    }

    void requireElement_(const ::jxx::Ptr<E>& element) const {
        if (element == nullptr) throw ::jxx::lang::NullPointerException();
        if (element->getDeclaringClass().get() != elementType_.get()) {
            throw ::jxx::lang::ClassCastException();
        }
        const auto ordinal = element->ordinal();
        if (ordinal < 0 ||
            static_cast<std::size_t>(ordinal) >= present_.size()) {
            throw ::jxx::lang::ClassCastException();
        }
    }

    class EnumSetIterator final
        : public ::jxx::lang::ClassBase<
              EnumSetIterator,
              ::jxx::lang::Object,
              Iterator<E>> {
        ::jxx::Ptr<EnumSet<E>> owner_;
        std::size_t next_ = 0U;
        std::size_t last_ = 0U;
        ::jxx::lang::jbool canRemove_ = false;

        void seek_() {
            while (next_ < owner_->present_.size() && !owner_->present_[next_]) {
                ++next_;
            }
        }

    public:
        explicit EnumSetIterator(const ::jxx::Ptr<EnumSet<E>>& owner)
            : owner_(owner) { seek_(); }

        ::jxx::lang::jbool hasNext() override {
            return next_ < owner_->present_.size();
        }

        ::jxx::Ptr<E> next() override {
            if (!hasNext()) throw NoSuchElementException();
            last_ = next_++;
            canRemove_ = true;
            seek_();
            return (*universe_())[static_cast<::jxx::lang::jint>(last_)];
        }

        void remove() override {
            if (!canRemove_) throw ::jxx::lang::IllegalStateException();
            if (owner_->present_[last_]) {
                owner_->present_[last_] = false;
                --owner_->size_;
                ++owner_->modCount_;
            }
            canRemove_ = false;
        }
    };

public:
    // ConstructionToken is private, so only EnumSet factories can call this.
    EnumSet(
        ConstructionToken,
        const ::jxx::Ptr<::jxx::lang::ClassAny>& elementType,
        ::jxx::lang::jint universeSize)
        : elementType_(elementType)
        , present_(static_cast<std::size_t>(universeSize), false) {}

    static ::jxx::Ptr<EnumSet<E>> noneOf(
        const ::jxx::Ptr<::jxx::lang::ClassAny>& elementType) {
        requireType_(elementType);
        const auto universe = universe_();
        return ::jxx::NEW<EnumSet<E>>(
            ConstructionToken{}, elementType, universe == nullptr ? 0 : universe->length);
    }

    static ::jxx::Ptr<EnumSet<E>> allOf(
        const ::jxx::Ptr<::jxx::lang::ClassAny>& elementType) {
        auto result = noneOf(elementType);
        for (std::size_t index = 0U; index < result->present_.size(); ++index) {
            result->present_[index] = true;
        }
        result->size_ = static_cast<::jxx::lang::jint>(result->present_.size());
        return result;
    }

    static ::jxx::Ptr<EnumSet<E>> of(const ::jxx::Ptr<E>& first) {
        if (first == nullptr) throw ::jxx::lang::NullPointerException();
        auto result = noneOf(first->getDeclaringClass());
        result->add(first);
        return result;
    }

    static ::jxx::Ptr<EnumSet<E>> of(
        const ::jxx::Ptr<E>& first,
        const ::jxx::Ptr<E>& second) {
        auto result = of(first);
        result->add(second);
        return result;
    }

    static ::jxx::Ptr<EnumSet<E>> range(
        const ::jxx::Ptr<E>& from,
        const ::jxx::Ptr<E>& to) {
        if (from == nullptr || to == nullptr) {
            throw ::jxx::lang::NullPointerException();
        }
        if (from->getDeclaringClass().get() != to->getDeclaringClass().get()) {
            throw ::jxx::lang::ClassCastException();
        }
        if (from->ordinal() > to->ordinal()) {
            throw ::jxx::lang::IllegalArgumentException();
        }
        auto result = noneOf(from->getDeclaringClass());
        for (::jxx::lang::jint ordinal = from->ordinal();
             ordinal <= to->ordinal(); ++ordinal) {
            result->present_[static_cast<std::size_t>(ordinal)] = true;
            ++result->size_;
        }
        return result;
    }

    static ::jxx::Ptr<EnumSet<E>> complementOf(
        const ::jxx::Ptr<EnumSet<E>>& source) {
        if (source == nullptr) throw ::jxx::lang::NullPointerException();
        auto result = noneOf(source->elementType_);
        for (std::size_t index = 0U; index < source->present_.size(); ++index) {
            result->present_[index] = !source->present_[index];
            if (result->present_[index]) ++result->size_;
        }
        return result;
    }

    static ::jxx::Ptr<EnumSet<E>> copyOf(
        const ::jxx::Ptr<EnumSet<E>>& source) {
        if (source == nullptr) throw ::jxx::lang::NullPointerException();
        auto result = noneOf(source->elementType_);
        result->present_ = source->present_;
        result->size_ = source->size_;
        return result;
    }

    ::jxx::lang::jbool add(const ::jxx::Ptr<E>& element) override {
        requireElement_(element);
        const auto index = static_cast<std::size_t>(element->ordinal());
        if (present_[index]) return false;
        present_[index] = true;
        ++size_;
        ++modCount_;
        return true;
    }

    ::jxx::lang::jbool contains(
        const ::jxx::Ptr<::jxx::lang::Object>& object) override {
        const auto value = ::jxx::CAST<E>(object);
        if (value == nullptr ||
            value->getDeclaringClass().get() != elementType_.get()) return false;
        const auto ordinal = value->ordinal();
        return ordinal >= 0 &&
            static_cast<std::size_t>(ordinal) < present_.size() &&
            present_[static_cast<std::size_t>(ordinal)];
    }

    ::jxx::lang::jbool remove(
        const ::jxx::Ptr<::jxx::lang::Object>& object) override {
        const auto value = ::jxx::CAST<E>(object);
        if (value == nullptr ||
            value->getDeclaringClass().get() != elementType_.get()) return false;
        const auto index = static_cast<std::size_t>(value->ordinal());
        if (index >= present_.size() || !present_[index]) return false;
        present_[index] = false;
        --size_;
        ++modCount_;
        return true;
    }

    ::jxx::lang::jint size() override { return size_; }

    void clear() override {
        if (size_ == 0) return;
        for (auto&& value : present_) value = false;
        size_ = 0;
        ++modCount_;
    }

    ::jxx::Ptr<Iterator<E>> iterator() override {
        return ::jxx::NEW<EnumSetIterator>(
            ::jxx::CAST<EnumSet<E>>(this->thisPtr()));
    }

    ::jxx::Ptr<::jxx::lang::Object> cloneImpl() const override {
        auto result = noneOf(elementType_);
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
            if (present_[index]) {
                output->writeObject(::jxx::CAST<::jxx::lang::Object>(
                    (*universe)[static_cast<::jxx::lang::jint>(index)]));
            }
        }
    }

    void readObject(
        const ::jxx::Ptr<::jxx::io::ObjectInputStream>& input) override {
        if (input == nullptr) throw ::jxx::lang::NullPointerException();
        elementType_ = E::Class();
        const auto universe = universe_();
        present_.assign(
            static_cast<std::size_t>(universe == nullptr ? 0 : universe->length), false);
        size_ = 0;
        const auto count = input->readInt();
        if (count < 0 || static_cast<std::size_t>(count) > present_.size()) {
            throw ::jxx::io::InvalidObjectException("Invalid EnumSet size");
        }
        for (::jxx::lang::jint index = 0; index < count; ++index) {
            const auto value = ::jxx::CAST<E>(input->readObject());
            if (value == nullptr) {
                throw ::jxx::io::InvalidObjectException("Invalid EnumSet element");
            }
            add(value);
        }
        modCount_ = 0;
    }

    void readObjectNoData() override {
        elementType_ = E::Class();
        const auto universe = universe_();
        present_.assign(
            static_cast<std::size_t>(universe == nullptr ? 0 : universe->length), false);
        size_ = 0;
        modCount_ = 0;
    }
};

} // namespace jxx::util
