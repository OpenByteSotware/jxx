#pragma once

#include <algorithm>
#include <vector>

#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.Cloneable.h"
#include "lang/jxx.lang.Exceptions.h"
#include "util/jxx.util.AbstractSet.h"
#include "util/jxx.util.ConcurrentModificationException.h"
#include "util/jxx.util.HashSet.h"
#include "util/jxx.util.Iterator.h"
#include "util/jxx.util.NoSuchElementException.h"

namespace jxx::util {

template <typename E>
class LinkedHashSet final
    : public AbstractSet<E>,
      public virtual ::jxx::lang::Cloneable,
      public virtual ::jxx::io::SerializableI {
public:
    using JxxSuper = AbstractSet<E>;

    LinkedHashSet() = default;
    explicit LinkedHashSet(::jxx::lang::jint initialCapacity) {
        if (initialCapacity < 0) throw ::jxx::lang::IllegalArgumentException();
        values_.reserve(static_cast<std::size_t>(initialCapacity));
    }
    LinkedHashSet(::jxx::lang::jint initialCapacity,
                  ::jxx::lang::jfloat loadFactor)
        : LinkedHashSet(initialCapacity) {
        if (!(loadFactor > 0.0F)) throw ::jxx::lang::IllegalArgumentException();
    }
    explicit LinkedHashSet(
        const ::jxx::Ptr<wildcard::CollectionExtends<E>>& collection) {
        if (collection == nullptr) throw ::jxx::lang::NullPointerException();
        addAll(collection);
    }

    ::jxx::lang::jint size() override {
        return static_cast<::jxx::lang::jint>(values_.size());
    }
    ::jxx::lang::jbool isEmpty() override { return values_.empty(); }
    ::jxx::lang::jbool contains(
        const ::jxx::Ptr<::jxx::lang::Object>& object) override {
        return indexOf_(object) >= 0;
    }
    ::jxx::lang::jbool add(const ::jxx::Ptr<E>& element) override {
        if (contains(::jxx::CAST<::jxx::lang::Object>(element))) return false;
        values_.push_back(element); ++modCount_; return true;
    }
    ::jxx::lang::jbool remove(
        const ::jxx::Ptr<::jxx::lang::Object>& object) override {
        const auto index = indexOf_(object);
        if (index < 0) return false;
        values_.erase(values_.begin() + index); ++modCount_; return true;
    }
    void clear() override {
        if (!values_.empty()) { values_.clear(); ++modCount_; }
    }

    void writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>&) override {
        throw ::jxx::lang::UnsupportedOperationException();
    }
    void readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>&) override {
        throw ::jxx::lang::UnsupportedOperationException();
    }
    void readObjectNoData() override {
        throw ::jxx::lang::UnsupportedOperationException();
    }

    ::jxx::Ptr<Iterator<E>> iterator() override {
        class OrderedIterator final : public Iterator<E> {
        public:
            explicit OrderedIterator(const ::jxx::Ptr<LinkedHashSet<E>>& owner)
                : owner_(owner), expected_(owner->modCount_) {}
            ::jxx::lang::jbool hasNext() override {
                check_(); return cursor_ < owner_->values_.size();
            }
            ::jxx::Ptr<E> next() override {
                check_();
                if (cursor_ >= owner_->values_.size()) throw NoSuchElementException();
                canRemove_ = true; return owner_->values_[cursor_++];
            }
            void remove() override {
                check_();
                if (!canRemove_) throw ::jxx::lang::IllegalStateException();
                owner_->values_.erase(owner_->values_.begin() + --cursor_);
                ++owner_->modCount_; expected_ = owner_->modCount_; canRemove_ = false;
            }
        private:
            void check_() const {
                if (expected_ != owner_->modCount_) throw ConcurrentModificationException();
            }
            ::jxx::Ptr<LinkedHashSet<E>> owner_;
            std::size_t cursor_ = 0;
            ::jxx::lang::jint expected_;
            bool canRemove_ = false;
        };
        return ::jxx::NEW<OrderedIterator>(
            ::jxx::CAST<LinkedHashSet<E>>(this->thisPtr()));
    }

    ::jxx::lang::ObjectArray toArray() override {
        auto result = ::jxx::NEW<::jxx::lang::ObjectArrayType>(
            static_cast<std::uint32_t>(values_.size()));
        for (std::size_t i=0;i<values_.size();++i)
            (*result)[static_cast<::jxx::lang::jint>(i)] =
                ::jxx::CAST<::jxx::lang::Object>(values_[i]);
        return result;
    }

protected:
    ::jxx::Ptr<::jxx::lang::Object> cloneImpl() const override {
        auto result=::jxx::NEW<LinkedHashSet<E>>();
        result->values_=values_;
        return ::jxx::CAST<::jxx::lang::Object>(result);
    }

private:
    ::jxx::lang::jint indexOf_(
        const ::jxx::Ptr<::jxx::lang::Object>& object) const {
        for (std::size_t i=0;i<values_.size();++i) {
            const auto current=::jxx::CAST<::jxx::lang::Object>(values_[i]);
            if (current==object || (current!=nullptr && current->equals(object)))
                return static_cast<::jxx::lang::jint>(i);
        }
        return -1;
    }
    std::vector<::jxx::Ptr<E>> values_;
    ::jxx::lang::jint modCount_=0;
};

} // namespace jxx::util
