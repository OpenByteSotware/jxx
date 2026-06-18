#pragma once

#include <set>

#include "util/jxx.util.AbstractSet.h"
#include "util/jxx.util.ComparatorSuper.h"
#include "lang/jxx.lang.Comparable.h"
#include "lang/jxx.lang.Cloneable.h"
#include "io/jxx.io.Serializable.h"

namespace jxx {
namespace util {

template <typename E>
class TreeSet
    : public AbstractSet<E>
    , public virtual jxx::lang::Cloneable
    , public virtual jxx::io::Serializable {
private:
    struct ElemLess {
        jxx::Ptr<ComparatorSuper<E>> comp_;

        bool operator()(const jxx::Ptr<E>& a, const jxx::Ptr<E>& b) const {
            if (a == nullptr || b == nullptr) {
                throw NullPointerException();
            }
            if (comp_ != nullptr) {
                return comp_->compare(a, b) < 0;
            }
            auto comparable = jxx::lang::ptr_checked_cast<jxx::lang::Comparable<E>>(a);
            if (comparable == nullptr) {
                throw ClassCastException();
            }
            return comparable->compareTo(b) < 0;
        }
    };

    using InternalSet = std::set<jxx::Ptr<E>, ElemLess>;

    InternalSet set_;
    jxx::Ptr<ComparatorSuper<E>> comparator_;
    jint modCount_;

public:
    TreeSet()
        : set_(ElemLess{nullptr})
        , comparator_(nullptr)
        , modCount_(0) {
    }

    explicit TreeSet(jxx::Ptr<ComparatorSuper<E>> comparator)
        : set_(ElemLess{comparator})
        , comparator_(comparator)
        , modCount_(0) {
    }

    explicit TreeSet(jxx::Ptr<wildcard::CollectionExtends<E>> c)
        : TreeSet() {
        if (c == nullptr) {
            throw NullPointerException();
        }
        addAll(c);
    }

    // Standalone approximation of TreeSet(SortedSet<E> s)
    explicit TreeSet(jxx::Ptr<TreeSet<E>> s)
        : TreeSet(s == nullptr ? jxx::Ptr<ComparatorSuper<E>>(nullptr) : s->comparator()) {
        if (s == nullptr) {
            throw NullPointerException();
        }
        auto it = s->iterator();
        while (it->hasNext()) {
            add(it->next());
        }
    }

    virtual ~TreeSet() = default;

    virtual jint size() override {
        return static_cast<jint>(set_.size());
    }

    virtual jbool isEmpty() override {
        return set_.empty();
    }

    virtual jxx::Ptr<ComparatorSuper<E>> comparator() {
        return comparator_;
    }

    virtual jbool contains(jxx::Ptr<jxx::lang::Object> o) override {
        auto castElem = jxx::lang::ptr_checked_cast<E>(o);
        if (castElem == nullptr) {
            return false;
        }
        return set_.find(castElem) != set_.end();
    }

    class TreeSetIterator : public virtual Iterator<E> {
    private:
        TreeSet<E>* setOwner_;
        typename InternalSet::iterator current_;
        typename InternalSet::iterator end_;
        jxx::Ptr<E> lastReturned_;
        jbool canRemove_;
        jint expectedModCount_;

    public:
        explicit TreeSetIterator(TreeSet<E>* setOwner)
            : setOwner_(setOwner)
            , current_(setOwner->set_.begin())
            , end_(setOwner->set_.end())
            , lastReturned_(nullptr)
            , canRemove_(false)
            , expectedModCount_(setOwner->modCount_) {
        }

        virtual ~TreeSetIterator() = default;

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
        return jxx::Ptr<Iterator<E>>(new TreeSetIterator(this));
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
        if (e == nullptr) {
            throw NullPointerException();
        }
        auto res = set_.insert(e);
        if (res.second) {
            ++modCount_;
            return true;
        }
        return false;
    }

    virtual jbool remove(jxx::Ptr<jxx::lang::Object> o) override {
        auto castElem = jxx::lang::ptr_checked_cast<E>(o);
        if (castElem == nullptr) {
            return false;
        }
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

    virtual jxx::Ptr<E> first() {
        if (set_.empty()) {
            throw NoSuchElementException();
        }
        return *set_.begin();
    }

    virtual jxx::Ptr<E> last() {
        if (set_.empty()) {
            throw NoSuchElementException();
        }
        auto it = set_.end();
        --it;
        return *it;
    }

    virtual jxx::Ptr<E> lower(jxx::Ptr<E> e) {
        if (e == nullptr) {
            throw NullPointerException();
        }
        auto it = set_.lower_bound(e);
        if (it == set_.begin()) {
            return nullptr;
        }
        if (it == set_.end() || !elemsEqual(*it, e)) {
            --it;
            return *it;
        }
        if (it == set_.begin()) {
            return nullptr;
        }
        --it;
        return *it;
    }

    virtual jxx::Ptr<E> floor(jxx::Ptr<E> e) {
        if (e == nullptr) {
            throw NullPointerException();
        }
        auto it = set_.upper_bound(e);
        if (it == set_.begin()) {
            return nullptr;
        }
        --it;
        return *it;
    }

    virtual jxx::Ptr<E> ceiling(jxx::Ptr<E> e) {
        if (e == nullptr) {
            throw NullPointerException();
        }
        auto it = set_.lower_bound(e);
        if (it == set_.end()) {
            return nullptr;
        }
        return *it;
    }

    virtual jxx::Ptr<E> higher(jxx::Ptr<E> e) {
        if (e == nullptr) {
            throw NullPointerException();
        }
        auto it = set_.upper_bound(e);
        if (it == set_.end()) {
            return nullptr;
        }
        return *it;
    }

    virtual jxx::Ptr<E> pollFirst() {
        if (set_.empty()) {
            return nullptr;
        }
        auto it = set_.begin();
        jxx::Ptr<E> value = *it;
        set_.erase(it);
        ++modCount_;
        return value;
    }

    virtual jxx::Ptr<E> pollLast() {
        if (set_.empty()) {
            return nullptr;
        }
        auto it = set_.end();
        --it;
        jxx::Ptr<E> value = *it;
        set_.erase(it);
        ++modCount_;
        return value;
    }

    virtual jxx::Ptr<jxx::lang::Object> clone() {
        jxx::Ptr<TreeSet<E>> cloned(new TreeSet<E>(comparator_));
        for (const auto& e : set_) {
            cloned->set_.insert(e);
        }
        return cloned;
    }

private:
    jbool elemsEqual(jxx::Ptr<E> a, jxx::Ptr<E> b) {
        if (a == nullptr) {
            return b == nullptr;
        }
        if (b == nullptr) {
            return false;
        }
        if (comparator_ != nullptr) {
            return comparator_->compare(a, b) == 0;
        }
        auto comparable = jxx::lang::ptr_checked_cast<jxx::lang::Comparable<E>>(a);
        if (comparable == nullptr) {
            throw ClassCastException();
        }
        return comparable->compareTo(b) == 0;
    }
};

} // namespace util
} // namespace jxx
