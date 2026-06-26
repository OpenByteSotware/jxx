#pragma once

#include <unordered_set>

#include "util/jxx.util.AbstractSet.h"
#include "lang/jxx.lang.Cloneable.h"
#include "io/jxx.io.Serializable.h"

namespace jxx {
namespace util {

template <typename E>
class HashSet
    : public AbstractSet<E>
    , public virtual jxx::lang::Cloneable
    , public virtual jxx::io::Serializable {
private:
    struct ElemHash {
        std::size_t operator()(const jxx::Ptr<E>& elem) const {
            if (elem == nullptr) {
                return 0u;
            }
            return static_cast<std::size_t>(elem->hashCode());
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
            return a->equals(b);
        }
    };

    using InternalSet = std::unordered_set<jxx::Ptr<E>, ElemHash, ElemEq>;

    static constexpr jint DEFAULT_INITIAL_CAPACITY = 16;
    static constexpr jfloat DEFAULT_LOAD_FACTOR = 0.75f;

    InternalSet set_;
    jfloat loadFactor_;
    jint threshold_;
    jint modCount_;

public:
    HashSet()
        : set_()
        , loadFactor_(DEFAULT_LOAD_FACTOR)
        , threshold_(DEFAULT_INITIAL_CAPACITY)
        , modCount_(0) {
        set_.reserve(static_cast<std::size_t>(DEFAULT_INITIAL_CAPACITY));
    }

    explicit HashSet(jint initialCapacity)
        : set_()
        , loadFactor_(DEFAULT_LOAD_FACTOR)
        , threshold_(initialCapacity > 0 ? initialCapacity : 0)
        , modCount_(0) {
        if (initialCapacity < 0) {
            throw IllegalArgumentException();
        }
        set_.reserve(static_cast<std::size_t>(initialCapacity));
    }

    HashSet(jint initialCapacity, jfloat loadFactor)
        : set_()
        , loadFactor_(loadFactor)
        , threshold_(initialCapacity > 0 ? initialCapacity : 0)
        , modCount_(0) {
        if (initialCapacity < 0) {
            throw IllegalArgumentException();
        }
        if (!(loadFactor > 0.0f) || loadFactor != loadFactor) {
            throw IllegalArgumentException();
        }
        set_.reserve(static_cast<std::size_t>(initialCapacity));
    }

    // Fresh standalone approximation of HashSet(Collection<? extends E> c)
    explicit HashSet(jxx::Ptr<wildcard::CollectionExtends<E>> c)
        : HashSet() {
        if (c == nullptr) {
            throw NullPointerException();
        }
        addAll(c);
    }

    virtual ~HashSet() = default;

    virtual jint size() override {
        return static_cast<jint>(set_.size());
    }

    virtual jbool isEmpty() override {
        return set_.empty();
    }

    virtual jbool contains(jxx::Ptr<jxx::lang::Object> o) override {
        return set_.find(jxx::CAST<E, jxx::lang::Object>(o)) != set_.end();
    }

    class HashSetIterator : public virtual jxx::util::Iterator<E> {
    private:
        HashSet<E>* setOwner_;
        typename InternalSet::iterator current_;
        typename InternalSet::iterator end_;
        jxx::Ptr<E> lastReturned_;
        jbool canRemove_;
        jint expectedModCount_;

    public:
        explicit HashSetIterator(HashSet<E>* setOwner)
            : setOwner_(setOwner)
            , current_(setOwner->set_.begin())
            , end_(setOwner->set_.end())
            , lastReturned_(nullptr)
            , canRemove_(false)
            , expectedModCount_(setOwner->modCount_) {
        }

        virtual ~HashSetIterator() = default;

        virtual jbool hasNext() override {
            return current_ != end_;
        }

        virtual jxx::Ptr<E> next() override {
            checkForComodification();
            if (current_ == end_) {
                throw NoSuchElementException();
            }
            lastReturned_ = *current_;
            ++current_;
            canRemove_ = true;
            return lastReturned_;
        }

        virtual void remove() override {
            if (!canRemove_) {
                throw IllegalStateException();
            }
            checkForComodification();
            setOwner_->remove(lastReturned_);
            expectedModCount_ = setOwner_->modCount_;
            canRemove_ = false;
            lastReturned_ = nullptr;
        }

    private:
        void checkForComodification() {
            if (setOwner_->modCount_ != expectedModCount_) {
                throw ConcurrentModificationException();
            }
        }
    };

    virtual jxx::Ptr<Iterator<E>> iterator() override {
        return jxx::Ptr<Iterator<E>>(new HashSetIterator(this));
    }

    virtual jxx::Ptr<JxxArray<jxx::Ptr<jxx::lang::Object>>> toArray() override {
        auto result = jxx::Ptr<JxxArray<jxx::Ptr<jxx::lang::Object>>>(
            new JxxArray<jxx::Ptr<jxx::lang::Object>>(size()));
        jint i = 0;
        for (const auto& e : set_) {
            result->set(i++, jxx::lang::ptr_static_cast<jxx::lang::Object>(e));
        }
        return result;
    }

    virtual jbool add(jxx::Ptr<E> e) override {
        auto res = set_.insert(e);
        if (res.second) {
            ++modCount_;
            resizeIfNeeded();
            return true;
        }
        return false;
    }

    virtual jbool remove(jxx::Ptr<jxx::lang::Object> o) override {
        auto castElem = jxx::CAST<E, jxx::lang::Object>(o);
        auto it = set_.find(castElem);
        if (it == set_.end()) {
            return false;
        }
        set_.erase(it);
        ++modCount_;
        return true;
    }

    virtual jbool containsAll(jxx::Ptr<wildcard::CollectionAny> c) override {
        return AbstractCollection<E>::containsAll(c);
    }

    virtual jbool addAll(jxx::Ptr<wildcard::CollectionExtends<E>> c) override {
        jbool modified = false;
        auto it = c->iteratorExtends();
        while (it->hasNext()) {
            if (add(it->next())) {
                modified = true;
            }
        }
        return modified;
    }

    virtual jbool retainAll(jxx::Ptr<wildcard::CollectionAny> c) override {
        jbool modified = false;
        auto it = iterator();
        while (it->hasNext()) {
            auto e = it->next();
            if (!c->containsObject(jxx::lang::ptr_static_cast<jxx::lang::Object>(e))) {
                it->remove();
                modified = true;
            }
        }
        return modified;
    }

    virtual jbool removeAll(jxx::Ptr<wildcard::CollectionAny> c) override {
        return AbstractSet<E>::removeAll(c);
    }

    virtual void clear() override {
        if (!set_.empty()) {
            set_.clear();
            ++modCount_;
        }
    }

    virtual jxx::Ptr<jxx::lang::Object> clone() {
        jxx::Ptr<HashSet<E>> cloned(new HashSet<E>(static_cast<jint>(set_.size()), loadFactor_));
        for (const auto& e : set_) {
            cloned->set_.insert(e);
        }
        cloned->threshold_ = threshold_;
        return cloned;
    }

private:
    void resizeIfNeeded() {
        if (size() > threshold_) {
            jint newThreshold = threshold_ <= 0 ? DEFAULT_INITIAL_CAPACITY : threshold_ << 1;
            threshold_ = newThreshold;
            set_.reserve(static_cast<std::size_t>(newThreshold));
        }
    }
};

} // namespace util
} // namespace jxx
