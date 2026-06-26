#pragma once

#include <cmath>
#include <cstddef>
#include <memory>
#include <unordered_set>

#include "lang/jxx.lang.Object.h"
#include "io/jxx.io.Serializable.h"
#include "lang/jxx.lang.Cloneable.h"
#include "lang/jxx.lang.Exceptions.h"
#include "util/jxx.util.AbstractCollection.h"
#include "util/jxx.util.AbstractSet.h"
#include "util/jxx.util.ConcurrentModificationException.h"
#include "util/jxx.util.Iterator.h"
#include "util/jxx.util.NoSuchElementException.h"

namespace jxx {
namespace util {

template <typename E>
class HashSet
    : public virtual AbstractSet<E>
    , public virtual jxx::lang::Cloneable
    , public virtual jxx::io::Serializable {
private:
    struct ElemHash {
        std::size_t operator()(const jxx::Ptr<E>& elem) const {
            if (elem == nullptr) {
                return 0u;
            }
            auto obj = jxx::CAST<jxx::lang::Object>(elem);
            return obj == nullptr ? 0u : static_cast<std::size_t>(obj->hashCode());
        }
    };

    struct ElemEq {
        bool operator()(const jxx::Ptr<E>& a, const jxx::Ptr<E>& b) const {
            if (a == nullptr) {
                return b == nullptr;
            }
            if (b == nullptr) {
                return false;
            }
            auto ao = jxx::CAST<jxx::lang::Object>(a);
            if (ao == nullptr) {
                return a.get() == b.get();
            }
            return ao->equals(jxx::CAST<jxx::lang::Object>(b));
        }
    };

    using InternalSet = std::unordered_set<jxx::Ptr<E>, ElemHash, ElemEq>;

    static constexpr jxx::lang::jint DEFAULT_INITIAL_CAPACITY = 16;
    static constexpr jxx::lang::jfloat DEFAULT_LOAD_FACTOR = 0.75f;
    static constexpr jxx::lang::jint MAXIMUM_CAPACITY = 1 << 30;

    InternalSet set_;
    jxx::lang::jfloat loadFactor_;
    jxx::lang::jint threshold_;
    jxx::lang::jint modCount_;

    static jxx::lang::jint tableSizeFor(jxx::lang::jint capacity) {
        if (capacity <= 0) {
            return 1;
        }
        if (capacity >= MAXIMUM_CAPACITY) {
            return MAXIMUM_CAPACITY;
        }
        jxx::lang::jint n = capacity - 1;
        n |= n >> 1;
        n |= n >> 2;
        n |= n >> 4;
        n |= n >> 8;
        n |= n >> 16;
        return n + 1;
    }

    static jxx::lang::jint thresholdFor(jxx::lang::jint capacity, jxx::lang::jfloat loadFactor) {
        if (capacity <= 0) {
            return 0;
        }
        const auto value = static_cast<double>(capacity) * static_cast<double>(loadFactor);
        if (value >= static_cast<double>(MAXIMUM_CAPACITY)) {
            return MAXIMUM_CAPACITY;
        }
        return static_cast<jxx::lang::jint>(value);
    }

    void resizeIfNeeded() {
        if (static_cast<jxx::lang::jint>(set_.size()) <= threshold_) {
            return;
        }
        const jxx::lang::jint current = static_cast<jxx::lang::jint>(set_.bucket_count());
        const jxx::lang::jint next = current <= 0 ? DEFAULT_INITIAL_CAPACITY : tableSizeFor(current << 1);
        threshold_ = thresholdFor(next, loadFactor_);
        set_.reserve(static_cast<std::size_t>(next));
    }

    static jxx::Ptr<E> castObjectToElement(jxx::Ptr<jxx::lang::Object> o) {
        return jxx::CAST<E>(o);
    }

public:
    class HashSetIterator final : public virtual Iterator<E> {
    private:
        HashSet<E>* owner_;
        typename InternalSet::iterator current_;
        jxx::Ptr<E> lastReturned_;
        jxx::lang::jbool canRemove_;
        jxx::lang::jint expectedModCount_;

        void checkForComodification() const {
            if (owner_->modCount_ != expectedModCount_) {
                throw jxx::util::ConcurrentModificationException();
            }
        }

    public:
        explicit HashSetIterator(HashSet<E>* owner)
            : owner_(owner),
              current_(owner->set_.begin()),
              lastReturned_(nullptr),
              canRemove_(static_cast<jxx::lang::jbool>(false)),
              expectedModCount_(owner->modCount_) {
        }

        virtual ~HashSetIterator() = default;

        virtual jxx::lang::jbool hasNext() override {
            return static_cast<jxx::lang::jbool>(current_ != owner_->set_.end());
        }

        virtual jxx::Ptr<E> next() override {
            checkForComodification();
            if (current_ == owner_->set_.end()) {
                throw jxx::util::NoSuchElementException();
            }
            lastReturned_ = *current_;
            ++current_;
            canRemove_ = static_cast<jxx::lang::jbool>(true);
            return lastReturned_;
        }

        virtual void remove() override {
            if (!canRemove_) {
                throw jxx::lang::IllegalStateException();
            }
            checkForComodification();
            owner_->remove(jxx::CAST<jxx::lang::Object>(lastReturned_));
            expectedModCount_ = owner_->modCount_;
            lastReturned_ = nullptr;
            canRemove_ = static_cast<jxx::lang::jbool>(false);
        }
    };

    HashSet()
        : set_(),
          loadFactor_(DEFAULT_LOAD_FACTOR),
          threshold_(thresholdFor(DEFAULT_INITIAL_CAPACITY, DEFAULT_LOAD_FACTOR)),
          modCount_(0) {
        set_.reserve(static_cast<std::size_t>(DEFAULT_INITIAL_CAPACITY));
    }

    explicit HashSet(jxx::lang::jint initialCapacity)
        : HashSet(initialCapacity, DEFAULT_LOAD_FACTOR) {
    }

    HashSet(jxx::lang::jint initialCapacity, jxx::lang::jfloat loadFactor)
        : set_(),
          loadFactor_(loadFactor),
          threshold_(0),
          modCount_(0) {
        if (initialCapacity < 0) {
            throw jxx::lang::IllegalArgumentException();
        }
        if (!(loadFactor > 0.0f) || std::isnan(static_cast<double>(loadFactor))) {
            throw jxx::lang::IllegalArgumentException();
        }
        const jxx::lang::jint capacity = tableSizeFor(initialCapacity);
        threshold_ = thresholdFor(capacity, loadFactor_);
        set_.reserve(static_cast<std::size_t>(capacity));
    }

    explicit HashSet(jxx::Ptr<wildcard::CollectionExtends<E>> c)
        : HashSet() {
        if (c == nullptr) {
            throw jxx::lang::NullPointerException();
        }
        addAll(c);
    }

    virtual ~HashSet() = default;

    // Serializable is a real pure-virtual interface in this JXX runtime.
    // Stream method APIs are not assumed here, so unsupported operations are conservative and compile-safe.
    virtual void writeObject(jxx::Ptr<jxx::io::ObjectOutputStream> out) override {
        if (out == nullptr) {
            throw jxx::lang::NullPointerException();
        }
        throw jxx::lang::UnsupportedOperationException();
    }

    virtual void readObject(jxx::Ptr<jxx::io::ObjectInputStream> in) override {
        if (in == nullptr) {
            throw jxx::lang::NullPointerException();
        }
        throw jxx::lang::UnsupportedOperationException();
    }

    virtual void readObjectNoData() override {
        throw jxx::lang::UnsupportedOperationException();
    }

    virtual jxx::Ptr<Iterator<E>> iterator() override {
        return jxx::Ptr<Iterator<E>>(new HashSetIterator(this));
    }

    virtual jxx::lang::jint size() override {
        return static_cast<jxx::lang::jint>(set_.size());
    }

    virtual jxx::lang::jbool isEmpty() override {
        return static_cast<jxx::lang::jbool>(set_.empty());
    }

    virtual jxx::lang::jbool contains(jxx::Ptr<jxx::lang::Object> o) override {
        return static_cast<jxx::lang::jbool>(set_.find(castObjectToElement(o)) != set_.end());
    }

    virtual jxx::Ptr<JxxArray<jxx::Ptr<jxx::lang::Object>, 1U>> toArray() override {
        auto result = jxx::Ptr<JxxArray<jxx::Ptr<jxx::lang::Object>, 1U>>(
            new JxxArray<jxx::Ptr<jxx::lang::Object>, 1U>(size()));
        jxx::lang::jint i = 0;
        for (const auto& e : set_) {
            result->set(i++, jxx::CAST<jxx::lang::Object>(e));
        }
        return result;
    }

    virtual jxx::lang::jbool add(jxx::Ptr<E> e) override {
        auto inserted = set_.insert(e);
        if (!inserted.second) {
            return static_cast<jxx::lang::jbool>(false);
        }
        ++modCount_;
        resizeIfNeeded();
        return static_cast<jxx::lang::jbool>(true);
    }

    virtual jxx::lang::jbool remove(jxx::Ptr<jxx::lang::Object> o) override {
        auto it = set_.find(castObjectToElement(o));
        if (it == set_.end()) {
            return static_cast<jxx::lang::jbool>(false);
        }
        set_.erase(it);
        ++modCount_;
        return static_cast<jxx::lang::jbool>(true);
    }

    virtual void clear() override {
        if (!set_.empty()) {
            set_.clear();
            ++modCount_;
        }
    }

    virtual jxx::lang::jbool containsAll(jxx::Ptr<wildcard::CollectionAny> c) override {
        if (c == nullptr) {
            throw jxx::lang::NullPointerException();
        }
        return AbstractCollection<E>::containsAll(c);
    }

    virtual jxx::lang::jbool addAll(jxx::Ptr<wildcard::CollectionExtends<E>> c) override {
        if (c == nullptr) {
            throw jxx::lang::NullPointerException();
        }
        jxx::lang::jbool modified = static_cast<jxx::lang::jbool>(false);
        auto it = c->iteratorExtends();
        while (it->hasNext()) {
            if (add(it->next())) {
                modified = static_cast<jxx::lang::jbool>(true);
            }
        }
        return modified;
    }

    virtual jxx::lang::jbool retainAll(jxx::Ptr<wildcard::CollectionAny> c) override {
        if (c == nullptr) {
            throw jxx::lang::NullPointerException();
        }
        jxx::lang::jbool modified = static_cast<jxx::lang::jbool>(false);
        auto it = iterator();
        while (it->hasNext()) {
            auto e = it->next();
            if (!c->containsObject(jxx::CAST<jxx::lang::Object>(e))) {
                it->remove();
                modified = static_cast<jxx::lang::jbool>(true);
            }
        }
        return modified;
    }

    virtual jxx::lang::jbool removeAll(jxx::Ptr<wildcard::CollectionAny> c) override {
        if (c == nullptr) {
            throw jxx::lang::NullPointerException();
        }
        return AbstractSet<E>::removeAll(c);
    }

    virtual jxx::Ptr<jxx::lang::Object> clone() {
        auto cloned = jxx::Ptr<HashSet<E>>(new HashSet<E>(static_cast<jxx::lang::jint>(set_.bucket_count()), loadFactor_));
        for (const auto& e : set_) {
            cloned->set_.insert(e);
        }
        cloned->threshold_ = threshold_;
        cloned->modCount_ = 0;
        return jxx::CAST<jxx::lang::Object>(cloned);
    }
};

} // namespace util
} // namespace jxx
