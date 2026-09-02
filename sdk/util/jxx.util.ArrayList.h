#pragma once

#include <cstdint>

#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Cloneable.h"
#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.buildin_array.h"
#include "util/jxx.util.AbstractList.h"
#include "util/jxx.util.ConcurrentModificationException.h"
#include "util/jxx.util.List.h"
#include "util/jxx.util.ListIterator.h"
#include "util/jxx.util.NoSuchElementException.h"
#include "util/jxx.util.RandomAccess.h"
#include "util/jxx.util.SubList.h"
#include "util/jxx.util.wildcard.CollectionExtends.h"

namespace jxx::util {

template <typename E>
class ArrayList
    : public jxx::lang::ClassBase<
          ArrayList<E>,
          AbstractList<E>,
          RandomAccess,
          jxx::lang::Cloneable,
          jxx::io::SerializableI> {
private:
    static constexpr jxx::lang::jint DEFAULT_CAPACITY = 10;

    jxx::Ptr<jxx::JxxArray<jxx::Ptr<E>, 1U>> elementData_;
    jxx::lang::jint size_ = 0;

    static jxx::lang::jint initialStorageCapacity_(
        jxx::lang::jint requestedCapacity) {

        return requestedCapacity > 0 ? requestedCapacity : 1;
    }

    void rangeCheck_(jxx::lang::jint index) const {
        if (index < 0 || index >= size_) {
            throw jxx::lang::IndexOutOfBoundsException();
        }
    }

    void rangeCheckForAdd_(jxx::lang::jint index) const {
        if (index < 0 || index > size_) {
            throw jxx::lang::IndexOutOfBoundsException();
        }
    }

    void ensureCapacityInternal_(jxx::lang::jint minimumCapacity) {
        if (minimumCapacity <= capacity_()) {
            return;
        }

        jxx::lang::jint newCapacity =
            capacity_() + (capacity_() >> 1);

        if (newCapacity < minimumCapacity) {
            newCapacity = minimumCapacity;
        }
        if (newCapacity < DEFAULT_CAPACITY) {
            newCapacity = DEFAULT_CAPACITY;
        }

        auto replacement =
            jxx::NEW<jxx::JxxArray<jxx::Ptr<E>, 1U>>(
                static_cast<std::uint32_t>(newCapacity));

        for (jxx::lang::jint index = 0; index < size_; ++index) {
            (*replacement)(index) = (*elementData_)(index);
        }

        elementData_ = replacement;
    }

    jxx::lang::jint capacity_() const {
        return static_cast<jxx::lang::jint>(elementData_->size());
    }

    void fastRemove_(jxx::lang::jint index) {
        const auto elementsToMove = size_ - index - 1;

        for (jxx::lang::jint offset = 0;
             offset < elementsToMove;
             ++offset) {

            (*elementData_)(index + offset) =
                (*elementData_)(index + offset + 1);
        }

        --size_;
        (*elementData_)(size_) = nullptr;
        ++this->modCount;
    }

    static jxx::lang::jbool equalsObject_(
        const jxx::Ptr<jxx::lang::Object>& left,
        const jxx::Ptr<jxx::lang::Object>& right) {

        return left == nullptr
            ? right == nullptr
            : left->equals(right);
    }

    class ArrayListIterator final
        : public jxx::lang::ClassBase<
              ArrayListIterator,
              jxx::lang::Object,
              ListIterator<E>> {
    private:
        jxx::Ptr<ArrayList<E>> owner_;
        jxx::lang::jint cursor_ = 0;
        jxx::lang::jint lastReturned_ = -1;
        jxx::lang::jint expectedModCount_ = 0;

        void checkForComodification_() const {
            if (expectedModCount_ != owner_->modCount) {
                throw ConcurrentModificationException();
            }
        }

    public:
        ArrayListIterator(
            const jxx::Ptr<ArrayList<E>>& owner,
            jxx::lang::jint index)
            : owner_(owner)
            , cursor_(index)
            , lastReturned_(-1)
            , expectedModCount_(owner == nullptr ? 0 : owner->modCount) {

            if (owner_ == nullptr) {
                throw jxx::lang::NullPointerException();
            }
            owner_->rangeCheckForAdd_(index);
        }

        ~ArrayListIterator() override = default;

        jxx::lang::jbool hasNext() override {
            return cursor_ < owner_->size_;
        }

        jxx::Ptr<E> next() override {
            checkForComodification_();
            if (!hasNext()) {
                throw NoSuchElementException();
            }

            lastReturned_ = cursor_;
            return (*owner_->elementData_)(cursor_++);
        }

        jxx::lang::jbool hasPrevious() override {
            return cursor_ > 0;
        }

        jxx::Ptr<E> previous() override {
            checkForComodification_();
            if (!hasPrevious()) {
                throw NoSuchElementException();
            }

            --cursor_;
            lastReturned_ = cursor_;
            return (*owner_->elementData_)(cursor_);
        }

        jxx::lang::jint nextIndex() override {
            return cursor_;
        }

        jxx::lang::jint previousIndex() override {
            return cursor_ - 1;
        }

        void remove() override {
            checkForComodification_();
            if (lastReturned_ < 0) {
                throw jxx::lang::IllegalStateException();
            }

            owner_->remove(lastReturned_);
            if (lastReturned_ < cursor_) {
                --cursor_;
            }
            lastReturned_ = -1;
            expectedModCount_ = owner_->modCount;
        }

        void set(const jxx::Ptr<E>& element) override {
            checkForComodification_();
            if (lastReturned_ < 0) {
                throw jxx::lang::IllegalStateException();
            }
            owner_->set(lastReturned_, element);
        }

        void add(const jxx::Ptr<E>& element) override {
            checkForComodification_();
            owner_->add(cursor_, element);
            ++cursor_;
            lastReturned_ = -1;
            expectedModCount_ = owner_->modCount;
        }
    };

public:
    ArrayList()
        : elementData_(
              jxx::NEW<jxx::JxxArray<jxx::Ptr<E>, 1U>>(
                  static_cast<std::uint32_t>(DEFAULT_CAPACITY))) {
    }

    explicit ArrayList(jxx::lang::jint initialCapacity)
        : elementData_(nullptr) {

        if (initialCapacity < 0) {
            throw jxx::lang::IllegalArgumentException();
        }

        elementData_ =
            jxx::NEW<jxx::JxxArray<jxx::Ptr<E>, 1U>>(
                static_cast<std::uint32_t>(
                    initialStorageCapacity_(initialCapacity)));
    }

    explicit ArrayList(
        const jxx::Ptr<wildcard::CollectionExtends<E>>& collection)
        : ArrayList(collection == nullptr ? 0 : collection->size()) {

        if (collection == nullptr) {
            throw jxx::lang::NullPointerException();
        }

        const auto iteratorValue = collection->iteratorExtends();
        while (iteratorValue->hasNext()) {
            add(iteratorValue->next());
        }

        this->modCount = 0;
    }

    ~ArrayList() override = default;

    void trimToSize() {
        const auto desiredCapacity =
            initialStorageCapacity_(size_);

        if (desiredCapacity == capacity_()) {
            return;
        }

        auto replacement =
            jxx::NEW<jxx::JxxArray<jxx::Ptr<E>, 1U>>(
                static_cast<std::uint32_t>(desiredCapacity));

        for (jxx::lang::jint index = 0; index < size_; ++index) {
            (*replacement)(index) = (*elementData_)(index);
        }

        elementData_ = replacement;
        ++this->modCount;
    }

    void ensureCapacity(jxx::lang::jint minimumCapacity) {
        if (minimumCapacity < 0) {
            throw jxx::lang::IllegalArgumentException();
        }
        ensureCapacityInternal_(minimumCapacity);
    }

    jxx::lang::jint size() override {
        return size_;
    }

    jxx::lang::jbool isEmpty() override {
        return size_ == 0;
    }

    jxx::lang::jbool contains(
        const jxx::Ptr<jxx::lang::Object>& object) override {

        return indexOf(object) >= 0;
    }

    jxx::lang::jint indexOf(
        const jxx::Ptr<jxx::lang::Object>& object) override {

        for (jxx::lang::jint index = 0; index < size_; ++index) {
            const auto candidate =
                jxx::CAST<jxx::lang::Object>((*elementData_)(index));

            if (equalsObject_(object, candidate)) {
                return index;
            }
        }
        return -1;
    }

    jxx::lang::jint lastIndexOf(
        const jxx::Ptr<jxx::lang::Object>& object) override {

        for (jxx::lang::jint index = size_ - 1; index >= 0; --index) {
            const auto candidate =
                jxx::CAST<jxx::lang::Object>((*elementData_)(index));

            if (equalsObject_(object, candidate)) {
                return index;
            }
        }
        return -1;
    }

    jxx::Ptr<E> get(jxx::lang::jint index) const override {
        rangeCheck_(index);
        return (*elementData_)(index);
    }

    jxx::Ptr<E> set(
        jxx::lang::jint index,
        const jxx::Ptr<E>& element) override {

        rangeCheck_(index);
        const auto oldValue = (*elementData_)(index);
        (*elementData_)(index) = element;
        return oldValue;
    }

    jxx::lang::jbool add(const jxx::Ptr<E>& element) override {
        ensureCapacityInternal_(size_ + 1);
        (*elementData_)(size_++) = element;
        ++this->modCount;
        return true;
    }

    void add(
        jxx::lang::jint index,
        const jxx::Ptr<E>& element) override {

        rangeCheckForAdd_(index);
        ensureCapacityInternal_(size_ + 1);

        for (jxx::lang::jint position = size_;
             position > index;
             --position) {

            (*elementData_)(position) =
                (*elementData_)(position - 1);
        }

        (*elementData_)(index) = element;
        ++size_;
        ++this->modCount;
    }

    jxx::Ptr<E> remove(jxx::lang::jint index) override {
        rangeCheck_(index);
        const auto oldValue = (*elementData_)(index);
        fastRemove_(index);
        return oldValue;
    }

    jxx::lang::jbool remove(
        const jxx::Ptr<jxx::lang::Object>& object) override {

        const auto index = indexOf(object);
        if (index < 0) {
            return false;
        }

        fastRemove_(index);
        return true;
    }

    void clear() override {
        if (size_ == 0) {
            return;
        }

        for (jxx::lang::jint index = 0; index < size_; ++index) {
            (*elementData_)(index) = nullptr;
        }

        size_ = 0;
        ++this->modCount;
    }

    jxx::lang::jbool addAll(
        const jxx::Ptr<wildcard::CollectionExtends<E>>& collection) override {

        return addAll(size_, collection);
    }

    jxx::lang::jbool addAll(
        jxx::lang::jint index,
        const jxx::Ptr<wildcard::CollectionExtends<E>>& collection) override {

        rangeCheckForAdd_(index);
        if (collection == nullptr) {
            throw jxx::lang::NullPointerException();
        }

        const auto count = collection->size();
        if (count == 0) {
            return false;
        }

        ensureCapacityInternal_(size_ + count);

        for (jxx::lang::jint position = size_ - 1;
             position >= index;
             --position) {

            (*elementData_)(position + count) =
                (*elementData_)(position);
        }

        auto iteratorValue = collection->iteratorExtends();
        auto destination = index;

        while (iteratorValue->hasNext()) {
            (*elementData_)(destination++) = iteratorValue->next();
        }

        size_ += count;
        ++this->modCount;
        return true;
    }

    jxx::Ptr<Iterator<E>> iterator() override {
        return jxx::CAST<Iterator<E>>(listIterator());
    }

    jxx::Ptr<ListIterator<E>> listIterator() override {
        return listIterator(0);
    }

    jxx::Ptr<ListIterator<E>> listIterator(
        jxx::lang::jint index) override {

        rangeCheckForAdd_(index);

        auto self =
            jxx::CAST<ArrayList<E>>(this->thisPtr);

        if (self == nullptr) {
            throw jxx::lang::IllegalStateException();
        }

        auto iteratorValue =
            jxx::NEW<ArrayListIterator>(self, index);

        return jxx::CAST<ListIterator<E>>(iteratorValue);
    }

    jxx::Ptr<List<E>> subList(
        jxx::lang::jint fromIndex,
        jxx::lang::jint toIndex) override {

        if (fromIndex < 0 || toIndex < fromIndex || toIndex > size_) {
            throw jxx::lang::IndexOutOfBoundsException();
        }

        auto self =
            jxx::CAST<ArrayList<E>>(this->thisPtr);

        if (self == nullptr) {
            throw jxx::lang::IllegalStateException();
        }

        auto root = jxx::CAST<List<E>>(self);
        auto view =
            jxx::NEW<SubList<E>>(root, fromIndex, toIndex);

        return jxx::CAST<List<E>>(view);
    }

    jxx::lang::ObjectArray toArray() override {
        auto result =
            jxx::NEW<jxx::lang::ObjectArrayType>(
                static_cast<std::uint32_t>(size_));

        for (jxx::lang::jint index = 0; index < size_; ++index) {
            (*result)[index] =
                jxx::CAST<jxx::lang::Object>((*elementData_)(index));
        }

        return result;
    }

protected:
    void removeRange(
        jxx::lang::jint fromIndex,
        jxx::lang::jint toIndex) {

        if (fromIndex < 0 || toIndex < fromIndex || toIndex > size_) {
            throw jxx::lang::IndexOutOfBoundsException();
        }

        const auto count = toIndex - fromIndex;
        if (count == 0) {
            return;
        }

        for (jxx::lang::jint position = toIndex;
             position < size_;
             ++position) {

            (*elementData_)(position - count) =
                (*elementData_)(position);
        }

        const auto newSize = size_ - count;
        for (jxx::lang::jint position = newSize;
             position < size_;
             ++position) {

            (*elementData_)(position) = nullptr;
        }

        size_ = newSize;
        ++this->modCount;
    }

    jxx::Ptr<jxx::lang::Object> cloneImpl() const override {
        auto result = jxx::NEW<ArrayList<E>>(size_);

        for (jxx::lang::jint index = 0; index < size_; ++index) {
            result->add((*elementData_)(index));
        }

        result->modCount = 0;
        return jxx::CAST<jxx::lang::Object>(result);
    }

public:
    void writeObject(
        const jxx::Ptr<jxx::io::ObjectOutputStream>& output) override {

        if (output == nullptr) {
            throw jxx::lang::NullPointerException();
        }
        throw jxx::lang::UnsupportedOperationException();
    }

    void readObject(
        const jxx::Ptr<jxx::io::ObjectInputStream>& input) override {

        if (input == nullptr) {
            throw jxx::lang::NullPointerException();
        }
        throw jxx::lang::UnsupportedOperationException();
    }

    void readObjectNoData() override {
        throw jxx::lang::UnsupportedOperationException();
    }
};

} // namespace jxx::util
