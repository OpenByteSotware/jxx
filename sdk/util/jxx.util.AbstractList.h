#pragma once

#include "util/jxx.util.AbstractCollection.h"
#include "util/jxx.util.List.h"
#include "util/jxx.util.Iterator.h"
#include "util/jxx.util.ListIterator.h"

namespace jxx {
namespace util {

template <typename E>
class AbstractList : public AbstractCollection<E>, public virtual List<E> {
protected:
    jint modCount = 0;

public:
    virtual ~AbstractList() = default;

    // ===== Concrete subclasses must implement =====
    virtual jxx::Ptr<E> get(jint index) override = 0;
    virtual jint size() override = 0;

    // ===== Optional operations =====
    virtual jxx::Ptr<E> set(jint index, jxx::Ptr<E> element) = 0;
        
    virtual void add(jint index, jxx::Ptr<E> element) = 0;
    

    virtual jxx::Ptr<E> remove(jint index) = 0;
    

    virtual jbool add(jxx::Ptr<E> e) override {
        add(size(), e);
        return true;
    }

    virtual jbool addAll(jxx::Ptr<wildcard::CollectionExtends<E>> c) override {
        return addAll(size(), c);
    }

    virtual jbool addAll(jint index, jxx::Ptr<wildcard::CollectionExtends<E>> c) override {
        rangeCheckForAdd(index);

        jbool modified = false;
        auto it = c->iteratorExtends();
        jint i = index;
        while (it->hasNext()) {
            add(i++, it->next());
            modified = true;
        }
        return modified;
    }

    virtual jint indexOf(jxx::Ptr<jxx::lang::Object> o) override {
        auto it = listIterator();
        if (o == nullptr) {
            while (it->hasNext()) {
                if (it->next() == nullptr) {
                    return it->previousIndex();
                }
            }
        } else {
            while (it->hasNext()) {
                auto e = it->next();
                if (e != nullptr && o->equals(jxx::lang::ptr_static_cast<jxx::lang::Object>(e))) {
                    return it->previousIndex();
                }
            }
        }
        return -1;
    }

    virtual jint lastIndexOf(jxx::Ptr<jxx::lang::Object> o) override {
        auto it = listIterator(size());
        if (o == nullptr) {
            while (it->hasPrevious()) {
                if (it->previous() == nullptr) {
                    return it->nextIndex();
                }
            }
        } else {
            while (it->hasPrevious()) {
                auto e = it->previous();
                if (e != nullptr && o->equals(jxx::lang::ptr_static_cast<jxx::lang::Object>(e))) {
                    return it->nextIndex();
                }
            }
        }
        return -1;
    }

    virtual jbool remove(jxx::Ptr<jxx::lang::Object> o) override {
        jint i = indexOf(o);
        if (i >= 0) {
            remove(i);
            return true;
        }
        return false;
    }

    virtual void clear() override {
        removeRange(0, size());
    }

    // Java List equality contract: ordered element-wise equality.
    virtual jbool equals(jxx::Ptr<jxx::lang::Object> o) override {
        auto c = jxx::lang::ptr_checked_cast<wildcard::CollectionAny>(o);
        if (c == nullptr) {
            return false;
        }
        if (c->size() != this->size()) {
            return false;
        }

        auto it1 = this->iterator();
        auto it2 = c->iteratorObject();
        while (it1->hasNext() && it2->hasNext()) {
            auto e1 = it1->next();
            auto e2 = it2->next();
            if (e1 == nullptr) {
                if (e2 != nullptr) {
                    return false;
                }
            } else {
                if (!e1->equals(e2)) {
                    return false;
                }
            }
        }
        return !it1->hasNext() && !it2->hasNext();
    }

    // Java List hashCode contract.
    virtual jint hashCode() override {
        jint hash = 1;
        auto it = this->iterator();
        while (it->hasNext()) {
            auto e = it->next();
            hash = 31 * hash + (e == nullptr ? 0 : e->hashCode());
        }
        return hash;
    }

    // ===== Iterators =====
    class Itr : public virtual Iterator<E> {
    protected:
        AbstractList<E>* list;
        jint cursor;
        jint lastRet;
        jint expectedModCount;

    public:
        explicit Itr(AbstractList<E>* l)
            : list(l), cursor(0), lastRet(-1), expectedModCount(l->modCount) {}

        virtual ~Itr() = default;

        virtual jbool hasNext() override {
            return cursor != list->size();
        }

        virtual jxx::Ptr<E> next() override {
            checkForComodification();
            const jint i = cursor;
            if (i >= list->size()) {
                throw NoSuchElementException();
            }
            cursor = i + 1;
            lastRet = i;
            return list->get(i);
        }

        virtual void remove() override {
            if (lastRet < 0) {
                throw IllegalStateException();
            }

            checkForComodification();
            list->remove(lastRet);
            if (lastRet < cursor) {
                cursor--;
            }
            lastRet = -1;
            expectedModCount = list->modCount;
        }

    protected:
        void checkForComodification() {
            if (list->modCount != expectedModCount) {
                throw ConcurrentModificationException();
            }
        }
    };

    class ListItr : public Itr, public virtual ListIterator<E> {
    public:
        ListItr(AbstractList<E>* l, jint index)
            : Itr(l) {
            this->cursor = index;
        }

        virtual ~ListItr() = default;

        virtual jbool hasPrevious() override {
            return this->cursor != 0;
        }

        virtual jxx::Ptr<E> previous() override {
            this->checkForComodification();
            const jint i = this->cursor - 1;
            if (i < 0) {
                throw NoSuchElementException();
            }
            this->cursor = i;
            this->lastRet = i;
            return this->list->get(i);
        }

        virtual jint nextIndex() override {
            return this->cursor;
        }

        virtual jint previousIndex() override {
            return this->cursor - 1;
        }

        virtual void set(jxx::Ptr<E> e) override {
            if (this->lastRet < 0) {
                throw IllegalStateException();
            }
            this->checkForComodification();
            this->list->set(this->lastRet, e);
            this->expectedModCount = this->list->modCount;
        }

        virtual void add(jxx::Ptr<E> e) override {
            this->checkForComodification();
            const jint i = this->cursor;
            this->list->add(i, e);
            this->cursor = i + 1;
            this->lastRet = -1;
            this->expectedModCount = this->list->modCount;
        }
    };

    virtual jxx::Ptr<Iterator<E>> iterator() override {
        return jxx::Ptr<Iterator<E>>(new Itr(this));
    }

    virtual jxx::Ptr<ListIterator<E>> listIterator() override {
        return listIterator(0);
    }

    virtual jxx::Ptr<ListIterator<E>> listIterator(jint index) override {
        rangeCheckForAdd(index);
        return jxx::Ptr<ListIterator<E>>(new ListItr(this, index));
    }

    virtual jxx::Ptr<List<E>> subList(jint /*fromIndex*/, jint /*toIndex*/) override {
        throw UnsupportedOperationException();
    }

protected:
    virtual void removeRange(jint fromIndex, jint toIndex) {
        auto it = listIterator(fromIndex);
        const jint n = toIndex - fromIndex;
        for (jint i = 0; i < n; ++i) {
            it->next();
            it->remove();
        }
    }

    void rangeCheckForAdd(jint index) {
        if (index < 0 || index > size()) {
            throw IndexOutOfBoundsException();
        }
    }
};

} // namespace util
} // namespace jxx
