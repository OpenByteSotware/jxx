#pragma once

#include "util/jxx.util.AbstractList.h"
#include "util/jxx.util.List.h"
#include "util/jxx.util.RandomAccess.h"
#include "util/jxx.util.SubList.h"
#include "lang/jxx.lang.Cloneable.h"
#include "io/jxx.io.Serializable.h"
#include "util/jxx.util.ComparatorSuper.h"
#include "util/jxx.util.Spliterator.h"
#include "util/function/jxx.util.function.PredicateSuper.h"
#include "util/function/jxx.util.function.UnaryOperator.h"
#include "util/function/jxx.util.function.Consumer.h"
#include "util/jxx.util.wildcard.CollectionExtends.h"
#include "lang/jxx.lang.Object.h"

namespace jxx {
namespace util {

template <typename E>
class ArrayList
    : public AbstractList<E>
    , public virtual RandomAccess
    , public virtual jxx::lang::Cloneable
    , public virtual jxx::io::Serializable {
private:
    static constexpr jint DEFAULT_CAPACITY = 10;

    jxx::Ptr<JxxArray<jxx::Ptr<E>>> elementData;
    jint size_;

public:
    ArrayList()
        : elementData(new JxxArray<jxx::Ptr<E>>(DEFAULT_CAPACITY)), size_(0) {
    }

    explicit ArrayList(jint initialCapacity)
        : elementData(), size_(0) {
        if (initialCapacity < 0) {
            throw IllegalArgumentException();
        }
        elementData = jxx::Ptr<JxxArray<jxx::Ptr<E>>>(new JxxArray<jxx::Ptr<E>>(initialCapacity));
    }

    // Java: ArrayList(Collection<? extends E> c)
    explicit ArrayList(jxx::Ptr<wildcard::CollectionExtends<E>> c)
        : elementData(), size_(0) {
        if (c == nullptr) {
            throw NullPointerException();
        }
        const jint n = c->size();
        elementData = jxx::Ptr<JxxArray<jxx::Ptr<E>>>(new JxxArray<jxx::Ptr<E>>(n > 0 ? n : DEFAULT_CAPACITY));
        auto it = c->iteratorExtends();
        while (it->hasNext()) {
            elementData->set(size_++, it->next());
        }
    }

    virtual ~ArrayList() = default;

    virtual void trimToSize() {
        if (size_ < elementData->length()) {
            elementData = copyArray(size_);
            ++this->modCount;
        }
    }

    virtual void ensureCapacity(jint minCapacity) {
        if (minCapacity > elementData->length()) {
            grow(minCapacity);
        }
    }

    virtual jint size() override {
        return size_;
    }

    virtual jbool isEmpty() override {
        return size_ == 0;
    }

    virtual jbool contains(jxx::Ptr<jxx::lang::Object> o) override {
        return indexOf(o) >= 0;
    }

    virtual jint indexOf(jxx::Ptr<jxx::lang::Object> o) override {
        if (o == nullptr) {
            for (jint i = 0; i < size_; ++i) {
                if (elementData->get(i) == nullptr) {
                    return i;
                }
            }
        } else {
            for (jint i = 0; i < size_; ++i) {
                auto e = elementData->get(i);
                if (e != nullptr && o->equals(jxx::lang::ptr_static_cast<jxx::lang::Object>(e))) {
                    return i;
                }
            }
        }
        return -1;
    }

    virtual jint lastIndexOf(jxx::Ptr<jxx::lang::Object> o) override {
        if (o == nullptr) {
            for (jint i = size_ - 1; i >= 0; --i) {
                if (elementData->get(i) == nullptr) {
                    return i;
                }
            }
        } else {
            for (jint i = size_ - 1; i >= 0; --i) {
                auto e = elementData->get(i);
                if (e != nullptr && o->equals(jxx::lang::ptr_static_cast<jxx::lang::Object>(e))) {
                    return i;
                }
            }
        }
        return -1;
    }

    virtual jxx::Ptr<jxx::lang::Object> clone() {
        jxx::Ptr<ArrayList<E>> cloned(new ArrayList<E>(size_));
        for (jint i = 0; i < size_; ++i) {
            cloned->elementData->set(i, elementData->get(i));
        }
        cloned->size_ = size_;
        return cloned;
    }

    virtual jxx::Ptr<JxxArray<jxx::Ptr<jxx::lang::Object>>> toArray() override {
        auto a = jxx::Ptr<JxxArray<jxx::Ptr<jxx::lang::Object>>>(
            new JxxArray<jxx::Ptr<jxx::lang::Object>>(size_));
        for (jint i = 0; i < size_; ++i) {
            a->set(i, elementData->get(i));
        }
        return a;
    }

    virtual jxx::Ptr<E> get(jint index) override {
        rangeCheck(index);
        return elementData->get(index);
    }

    virtual jxx::Ptr<E> set(jint index, jxx::Ptr<E> element) override {
        rangeCheck(index);
        jxx::Ptr<E> oldValue = elementData->get(index);
        elementData->set(index, element);
        return oldValue;
    }

    virtual jbool add(jxx::Ptr<E> e) override {
        ensureCapacityInternal(size_ + 1);
        elementData->set(size_++, e);
        ++this->modCount;
        return true;
    }

    virtual void add(jint index, jxx::Ptr<E> element) override {
        rangeCheckForAdd(index);
        ensureCapacityInternal(size_ + 1);
        shiftRight(index, 1);
        elementData->set(index, element);
        ++size_;
        ++this->modCount;
    }

    virtual jxx::Ptr<E> remove(jint index) override {
        rangeCheck(index);
        ++this->modCount;
        jxx::Ptr<E> oldValue = elementData->get(index);
        shiftLeft(index + 1, 1);
        --size_;
        elementData->set(size_, nullptr);
        return oldValue;
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
        ++this->modCount;
        for (jint i = 0; i < size_; ++i) {
            elementData->set(i, nullptr);
        }
        size_ = 0;
    }

    virtual jbool addAll(jxx::Ptr<wildcard::CollectionExtends<E>> c) override {
        return addAll(size_, c);
    }

    virtual jbool addAll(jint index, jxx::Ptr<wildcard::CollectionExtends<E>> c) override {
        rangeCheckForAdd(index);
        if (c == nullptr) {
            throw NullPointerException();
        }
        const jint numNew = c->size();
        if (numNew == 0) {
            return false;
        }
        ensureCapacityInternal(size_ + numNew);
        shiftRight(index, numNew);
        auto it = c->iteratorExtends();
        jint dest = index;
        while (it->hasNext()) {
            elementData->set(dest++, it->next());
        }
        size_ += numNew;
        ++this->modCount;
        return true;
    }

    virtual jxx::Ptr<List<E>> subList(jint fromIndex, jint toIndex) override {
        if (fromIndex < 0 || toIndex > size_ || fromIndex > toIndex) {
            throw IndexOutOfBoundsException();
        }
        return jxx::Ptr<List<E>>(new SubList<E>(jxx::Ptr<List<E>>(this), fromIndex, toIndex));
    }

    virtual jbool removeIf(jxx::Ptr<function::PredicateSuper<E>> filter) override {
        if (filter == nullptr) {
            throw NullPointerException();
        }
        jbool removed = false;
        jint i = 0;
        while (i < size_) {
            if (filter->test(elementData->get(i))) {
                remove(i);
                removed = true;
            } else {
                ++i;
            }
        }
        return removed;
    }

    virtual void replaceAll(jxx::Ptr<function::UnaryOperator<E>> op) override {
        if (op == nullptr) {
            throw NullPointerException();
        }
        const jint expected = this->modCount;
        for (jint i = 0; i < size_; ++i) {
            elementData->set(i, op->apply(elementData->get(i)));
        }
        if (this->modCount != expected) {
            throw ConcurrentModificationException();
        }
        ++this->modCount;
    }

    virtual void sort(jxx::Ptr<ComparatorSuper<E>> c) override {
        if (c == nullptr) {
            throw NullPointerException();
        }
        const jint expected = this->modCount;
        for (jint i = 1; i < size_; ++i) {
            jxx::Ptr<E> key = elementData->get(i);
            jint j = i - 1;
            while (j >= 0 && c->compare(elementData->get(j), key) > 0) {
                elementData->set(j + 1, elementData->get(j));
                --j;
            }
            elementData->set(j + 1, key);
        }
        if (this->modCount != expected) {
            throw ConcurrentModificationException();
        }
        ++this->modCount;
    }

    class ArrayListSpliterator : public virtual Spliterator<E> {
    private:
        ArrayList<E>* list;
        jint index;
        jint fence;
        jint expectedModCount;

    public:
        ArrayListSpliterator(ArrayList<E>* l, jint origin, jint fenceIndex, jint expected)
            : list(l), index(origin), fence(fenceIndex), expectedModCount(expected) {}

        virtual ~ArrayListSpliterator() = default;

        virtual jbool tryAdvance(jxx::Ptr<function::Consumer<E>> action) override {
            if (action == nullptr) {
                throw NullPointerException();
            }
            if (index < fence) {
                action->accept(list->elementData->get(index++));
                checkForComodification();
                return true;
            }
            return false;
        }

        virtual void forEachRemaining(jxx::Ptr<function::Consumer<E>> action) override {
            if (action == nullptr) {
                throw NullPointerException();
            }
            while (index < fence) {
                action->accept(list->elementData->get(index++));
            }
            checkForComodification();
        }

        virtual jxx::Ptr<Spliterator<E>> trySplit() override {
            const jint lo = index;
            const jint mid = lo + ((fence - lo) >> 1);
            if (lo >= mid) {
                return nullptr;
            }
            index = mid;
            return jxx::Ptr<Spliterator<E>>(new ArrayListSpliterator(list, lo, mid, expectedModCount));
        }

        virtual jlong estimateSize() override {
            return static_cast<jlong>(fence - index);
        }

        virtual jint characteristics() override {
            return Spliterator<E>::ORDERED | Spliterator<E>::SIZED | Spliterator<E>::SUBSIZED;
        }

    private:
        void checkForComodification() {
            if (list->modCount != expectedModCount) {
                throw ConcurrentModificationException();
            }
        }
    };

    virtual jxx::Ptr<Spliterator<E>> spliterator() override {
        return jxx::Ptr<Spliterator<E>>(new ArrayListSpliterator(this, 0, size_, this->modCount));
    }

protected:
    virtual void removeRange(jint fromIndex, jint toIndex) override {
        if (fromIndex < 0 || toIndex > size_ || fromIndex > toIndex) {
            throw IndexOutOfBoundsException();
        }
        ++this->modCount;
        const jint numMoved = size_ - toIndex;
        for (jint i = 0; i < numMoved; ++i) {
            elementData->set(fromIndex + i, elementData->get(toIndex + i));
        }
        const jint newSize = size_ - (toIndex - fromIndex);
        for (jint i = newSize; i < size_; ++i) {
            elementData->set(i, nullptr);
        }
        size_ = newSize;
    }

private:
    void ensureCapacityInternal(jint minCapacity) {
        if (elementData == nullptr) {
            elementData = jxx::Ptr<JxxArray<jxx::Ptr<E>>>(new JxxArray<jxx::Ptr<E>>(DEFAULT_CAPACITY));
        }
        if (minCapacity > elementData->length()) {
            grow(minCapacity);
        }
    }

    void grow(jint minCapacity) {
        jint oldCapacity = elementData->length();
        jint newCapacity = oldCapacity + (oldCapacity >> 1);
        if (newCapacity < minCapacity) {
            newCapacity = minCapacity;
        }
        if (newCapacity < DEFAULT_CAPACITY) {
            newCapacity = DEFAULT_CAPACITY;
        }
        elementData = copyArray(newCapacity);
        ++this->modCount;
    }

    jxx::Ptr<JxxArray<jxx::Ptr<E>>> copyArray(jint newCapacity) {
        auto newData = jxx::Ptr<JxxArray<jxx::Ptr<E>>>(new JxxArray<jxx::Ptr<E>>(newCapacity));
        const jint limit = (size_ < newCapacity) ? size_ : newCapacity;
        for (jint i = 0; i < limit; ++i) {
            newData->set(i, elementData->get(i));
        }
        return newData;
    }

    void shiftRight(jint index, jint count) {
        for (jint i = size_ - 1; i >= index; --i) {
            elementData->set(i + count, elementData->get(i));
        }
    }

    void shiftLeft(jint fromIndex, jint count) {
        for (jint i = fromIndex; i < size_; ++i) {
            elementData->set(i - count, elementData->get(i));
        }
    }

    void rangeCheck(jint index) {
        if (index < 0 || index >= size_) {
            throw IndexOutOfBoundsException();
        }
    }

    void rangeCheckForAdd(jint index) {
        if (index < 0 || index > size_) {
            throw IndexOutOfBoundsException();
        }
    }
};

} // namespace util
} // namespace jxx
