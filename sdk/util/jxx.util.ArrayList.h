#pragma once
#include "lang/jxx.lang.Object.h"
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
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace jxx {
namespace util {

template <typename E>
class ArrayList
    : public virtual AbstractList<E>
    , public virtual RandomAccess
    , public virtual jxx::lang::Cloneable
    , public virtual jxx::io::Serializable {
private:
    static constexpr jxx::lang::jint DEFAULT_CAPACITY = 10;

    jxx::Ptr<JxxArray<jxx::Ptr<E>, 1U>> elementData;
    jxx::lang::jint size_;

public:
    ArrayList()
        : elementData(new JxxArray<jxx::Ptr<E>, 1U>(DEFAULT_CAPACITY)), size_(0) {
    }

    explicit ArrayList(jxx::lang::jint initialCapacity)
        : elementData(), size_(0) {
        if (initialCapacity < 0) {
            throw jxx::lang::IllegalArgumentException();
        }
        elementData = jxx::Ptr<JxxArray<jxx::Ptr<E>, 1U>>(jxx::NEW<JxxArray<jxx::Ptr<E>, 1U>>(initialCapacity));
    }

    // Java: ArrayList(Collection<? extends E> c)
    explicit ArrayList(jxx::Ptr<wildcard::CollectionExtends<E>> c)
        : elementData(), size_(0) {
        if (c == nullptr) {
            throw jxx::lang::NullPointerException();
        }
        const jxx::lang::jint n = c->size();
        elementData = jxx::Ptr<JxxArray<jxx::Ptr<E>>>(new JxxArray<jxx::Ptr<E>, 1U>(n > 0 ? n : DEFAULT_CAPACITY));
        auto it = c->iteratorExtends();
        while (it->hasNext()) {
            (*elementData)(size_++) = it->next();
        }
    }

    virtual ~ArrayList() = default;

    virtual void writeObject(jxx::Ptr<jxx::io::ObjectOutputStream> out) override {

    }
    virtual void readObject(jxx::Ptr<jxx::io::ObjectInputStream> in) override {

    }
    virtual void readObjectNoData() override {
	}

    virtual jxx::Ptr<jxx::lang::Object> cloneImpl() const override {
        return jxx::NEW<ArrayList<E>>(*this);
    }

    virtual void trimToSize() {
        if (size_ < elementData->size()) {
            elementData = copyArray(size_);
            ++this->modCount;
        }
    }

    virtual void ensureCapacity(jxx::lang::jint minCapacity) {
        if (minCapacity > elementData->size()) {
            grow(minCapacity);
        }
    }

    virtual jxx::lang::jint size() override {
        return size_;
    }

    virtual jxx::lang::jbool isEmpty() override {
        return size_ == 0;
    }

    virtual jxx::lang::jbool contains(jxx::Ptr<jxx::lang::Object> o) override {
        return indexOf(o) >= 0;
    }

    virtual jxx::lang::jint indexOf(jxx::Ptr<jxx::lang::Object> o) override {
        if (o == nullptr) {
            for (jxx::lang::jint i = 0; i < size_; ++i) {
                if ((*elementData)(i) == nullptr) {
                    return i;
                }
            }
        } else {
            for (jxx::lang::jint i = 0; i < size_; ++i) {
                auto e = (*elementData)(i);
                if (e != nullptr && o->equals(jxx::CAST<jxx::lang::Object, E>(e))) {
                    return i;
                }
            }
        }
        return -1;
    }

    virtual jxx::lang::jint lastIndexOf(jxx::Ptr<jxx::lang::Object> o) override {
        if (o == nullptr) {
            for (jxx::lang::jint i = size_ - 1; i >= 0; --i) {
                if ((*elementData)(i) == nullptr) {
                    return i;
                }
            }
        } else {
            for (jxx::lang::jint i = size_ - 1; i >= 0; --i) {
                auto e = (*elementData)(i);
                if (e != nullptr && o->equals(jxx::CAST<jxx::lang::Object, E>(e))) {
                    return i;
                }
            }
        }
        return -1;
    }

    virtual jxx::Ptr<jxx::lang::Object> clone() {
        jxx::Ptr<ArrayList<E>> cloned(jxx::NEW<ArrayList<E>>(size_));
        for (jxx::lang::jint i = 0; i < size_; ++i) {
            (*cloned->elementData)(i) = (*elementData)(i);
        }
        cloned->size_ = size_;
        return cloned;
    }

    virtual jxx::Ptr<JxxArray<jxx::Ptr<jxx::lang::Object>, 1U>> toArray() override {
        auto a = jxx::Ptr<JxxArray<jxx::Ptr<jxx::lang::Object>, 1U>>(jxx::NEW<JxxArray<jxx::Ptr<jxx::lang::Object>, 1U>>(size_));
        for (jxx::lang::jint i = 0; i < size_; ++i) {
            (*a)(i) = (*elementData)(i);
        }
        return a;
    }

    virtual jxx::Ptr<E> get(jxx::lang::jint index) override {
        rangeCheck(index);
        return (*elementData)(index);
    }

    virtual jxx::Ptr<E> set(jxx::lang::jint index, const jxx::Ptr<E> element) override {
        rangeCheck(index);
        jxx::Ptr<E> oldValue = (*elementData)(index);
        (*elementData)(index) = element;
        return oldValue;
    }

    virtual jxx::lang::jbool add(const jxx::Ptr<E> e) override {
        ensureCapacityInternal(size_ + 1);
        (*elementData)(size_++) = e;
        ++this->modCount;
        return true;
    }

    virtual void add(jxx::lang::jint index, const jxx::Ptr<E> element) override {
        rangeCheckForAdd(index);
        ensureCapacityInternal(size_ + 1);
        shiftRight(index, 1);
        (*elementData)(index) = element;
        ++size_;
        ++this->modCount;
    }

    virtual jxx::Ptr<E> remove(jxx::lang::jint index) override {
        rangeCheck(index);
        ++this->modCount;
        jxx::Ptr<E> oldValue = (*elementData)(index);
        shiftLeft(index + 1, 1);
        --size_;
        (*elementData)(size_) = nullptr;
        return oldValue;
    }

    virtual jxx::lang::jbool remove(jxx::Ptr<jxx::lang::Object> o) override {
        jxx::lang::jint i = indexOf(o);
        if (i >= 0) {
            remove(i);
            return true;
        }
        return false;
    }

    virtual void clear() override {
        ++this->modCount;
        for (jxx::lang::jint i = 0; i < size_; ++i) {
            (*elementData)(i) = nullptr;
        }
        size_ = 0;
    }

    virtual jxx::lang::jbool addAll(jxx::Ptr<wildcard::CollectionExtends<E>> c) override {
        return addAll(size_, c);
    }

    virtual jxx::lang::jbool addAll(jxx::lang::jint index, jxx::Ptr<wildcard::CollectionExtends<E>> c) override {
        rangeCheckForAdd(index);
        if (c == nullptr) {
            throw jxx::lang::NullPointerException();
        }
        const jxx::lang::jint numNew = c->size();
        if (numNew == 0) {
            return false;
        }
        ensureCapacityInternal(size_ + numNew);
        shiftRight(index, numNew);
        auto it = c->iteratorExtends();
        jxx::lang::jint dest = index;
        while (it->hasNext()) {
            (*elementData)(dest++) = it->next();
        }
        size_ += numNew;
        ++this->modCount;
        return true;
    }

    virtual jxx::Ptr<List<E>> subList(jxx::lang::jint fromIndex, jxx::lang::jint toIndex) override {
        if (fromIndex < 0 || toIndex > size_ || fromIndex > toIndex) {
            throw jxx::lang::IndexOutOfBoundsException();
        }
        return jxx::Ptr<List<E>>(jxx::NEW<SubList<E>>(jxx::Ptr<List<E>>(this), fromIndex, toIndex));
    }

    virtual jxx::lang::jbool removeIf(jxx::Ptr<function::PredicateSuper<E>> filter) override {
        if (filter == nullptr) {
            throw jxx::lang::NullPointerException();
        }
        jxx::lang::jbool removed = false;
        jxx::lang::jint i = 0;
        while (i < size_) {
            if (filter->test((*elementData)(i))) {
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
            throw jxx::lang::NullPointerException();
        }
        const jxx::lang::jint expected = this->modCount;
        for (jxx::lang::jint i = 0; i < size_; ++i) {
            (*elementData)(i) = op->apply((*elementData)(i));
        }
        if (this->modCount != expected) {
            throw ConcurrentModificationException();
        }
        ++this->modCount;
    }

    virtual void sort(jxx::Ptr<ComparatorSuper<E>> c) override {
        if (c == nullptr) {
            throw jxx::lang::NullPointerException();
        }
        const jxx::lang::jint expected = this->modCount;
        for (jxx::lang::jint i = 1; i < size_; ++i) {
            jxx::Ptr<E> key = (*elementData)(i);
            jxx::lang::jint j = i - 1;
            while (j >= 0 && c->compareSuper((*elementData)(j), key) > 0) {
                (*elementData)(j + 1) = (*elementData)(j);
                --j;
            }
            (*elementData)(j + 1) = key;
        }
        if (this->modCount != expected) {
            throw ConcurrentModificationException();
        }
        ++this->modCount;
    }

    class ArrayListSpliterator : public virtual Spliterator<E> {
    private:
        ArrayList<E>* list;
        jxx::lang::jint index;
        jxx::lang::jint fence;
        jxx::lang::jint expectedModCount;

    public:
        ArrayListSpliterator(ArrayList<E>* l, jxx::lang::jint origin, jxx::lang::jint fenceIndex, jxx::lang::jint expected)
            : list(l), index(origin), fence(fenceIndex), expectedModCount(expected) {}

        virtual ~ArrayListSpliterator() = default;

        virtual jxx::lang::jbool tryAdvance(jxx::Ptr<function::Consumer<E>> action) override {
            if (action == nullptr) {
                throw jxx::lang::NullPointerException();
            }
            if (index < fence) {
				auto e = list->elementData;
                action->accept((*e)(index++));
                checkForComodification();
                return true;
            }
            return false;
        }

        virtual void forEachRemaining(jxx::Ptr<function::Consumer<E>> action) override {
            if (action == nullptr) {
                throw jxx::lang::NullPointerException();
            }
            while (index < fence) {
                auto e = list->elementData;
                action->accept((*e)(index++));
            }
            checkForComodification();
        }

        virtual jxx::Ptr<Spliterator<E>> trySplit() override {
            const jxx::lang::jint lo = index;
            const jxx::lang::jint mid = lo + ((fence - lo) >> 1);
            if (lo >= mid) {
                return nullptr;
            }
            index = mid;
            return jxx::Ptr<Spliterator<E>>(new ArrayListSpliterator(list, lo, mid, expectedModCount));
        }

        virtual jxx::lang::jlong estimateSize() override {
            return static_cast<jxx::lang::jlong>(fence - index);
        }

        virtual jxx::lang::jint characteristics() override {
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
    virtual void removeRange(jxx::lang::jint fromIndex, jxx::lang::jint toIndex) override {
        if (fromIndex < 0 || toIndex > size_ || fromIndex > toIndex) {
            throw jxx::lang::IndexOutOfBoundsException();
        }
        ++this->modCount;
        const jxx::lang::jint numMoved = size_ - toIndex;
        for (jxx::lang::jint i = 0; i < numMoved; ++i) {
            (*elementData)(fromIndex + i) = (*elementData)(toIndex + i);
        }
        const jxx::lang::jint newSize = size_ - (toIndex - fromIndex);
        for (jxx::lang::jint i = newSize; i < size_; ++i) {
            (*elementData)(i) = nullptr;
        }
        size_ = newSize;
    }

private:
    void ensureCapacityInternal(jxx::lang::jint minCapacity) {
        if (elementData == nullptr) {
            elementData = jxx::NEW<JxxArray<jxx::Ptr<E>, 1U>>(DEFAULT_CAPACITY);
        }
        if (minCapacity > elementData->size()) {
            grow(minCapacity);
        }
    }

    void grow(jxx::lang::jint minCapacity) {
        jxx::lang::jint oldCapacity = elementData->size();
        jxx::lang::jint newCapacity = oldCapacity + (oldCapacity >> 1);
        if (newCapacity < minCapacity) {
            newCapacity = minCapacity;
        }
        if (newCapacity < DEFAULT_CAPACITY) {
            newCapacity = DEFAULT_CAPACITY;
        }
        elementData = copyArray(newCapacity);
        ++this->modCount;
    }

    jxx::Ptr<JxxArray<jxx::Ptr<E>, 1U>> copyArray(jxx::lang::jint newCapacity) {
        auto newData = jxx::NEW<JxxArray<jxx::Ptr<E>, 1U>>(newCapacity);
        const jxx::lang::jint limit = (size_ < newCapacity) ? size_ : newCapacity;
        for (jxx::lang::jint i = 0; i < limit; ++i) {
            (*newData)(i) = (*elementData)(i);
        }
        return newData;
    }

    void shiftRight(jxx::lang::jint index, jxx::lang::jint count) {
        for (jxx::lang::jint i = size_ - 1; i >= index; --i) {
            (*elementData)(i + count) = (*elementData)(i);
        }
    }

    void shiftLeft(jxx::lang::jint fromIndex, jxx::lang::jint count) {
        for (jxx::lang::jint i = fromIndex; i < size_; ++i) {
            (*elementData)(i - count) = (*elementData)(i);
        }
    }

    void rangeCheck(jxx::lang::jint index) {
        if (index < 0 || index >= size_) {
            throw jxx::lang::IndexOutOfBoundsException();
        }
    }

    void rangeCheckForAdd(jxx::lang::jint index) {
        if (index < 0 || index > size_) {
            throw jxx::lang::IndexOutOfBoundsException();
        }
    }
};

} // namespace util
} // namespace jxx
