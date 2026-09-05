#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Exceptions.h"
#include "util/jxx.util.AbstractList.h"
#include "util/jxx.util.ConcurrentModificationException.h"
#include "util/jxx.util.List.h"
#include "util/jxx.util.ListIterator.h"
#include "util/jxx.util.NoSuchElementException.h"
#include "util/jxx.util.RandomAccess.h"

namespace jxx::util {

template <typename E>
class SubList
    : public jxx::lang::ClassBase<
          SubList<E>,
          AbstractList<E>,
          RandomAccess> {
private:
    jxx::Ptr<List<E>> root_;
    jxx::lang::jint offset_ = 0;
    jxx::lang::jint size_ = 0;
    jxx::lang::jint expectedRootSize_ = 0;

    void checkForComodification_() const {
        if (root_->size() != expectedRootSize_) {
            throw ConcurrentModificationException();
        }
    }

    void updateAfterStructuralChange_(jxx::lang::jint delta) {
        size_ += delta;
        expectedRootSize_ += delta;
        ++this->modCount;
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

    static jxx::lang::jbool equalsObject_(
        const jxx::Ptr<jxx::lang::Object>& left,
        const jxx::Ptr<jxx::lang::Object>& right) {

        return left == nullptr
            ? right == nullptr
            : left->equals(right);
    }

    class SubListIterator final
        : public jxx::lang::ClassBase<
              SubListIterator,
              jxx::lang::Object,
              ListIterator<E>> {
    private:
        jxx::Ptr<SubList<E>> owner_;
        jxx::Ptr<ListIterator<E>> rootIterator_;
        jxx::lang::jint cursor_ = 0;
        jxx::lang::jint lastReturned_ = -1;
        jxx::lang::jint expectedRootSize_ = 0;

        void checkForComodification_() const {
            if (owner_->root_->size() != expectedRootSize_) {
                throw ConcurrentModificationException();
            }
        }

    public:
        SubListIterator(
            const jxx::Ptr<SubList<E>>& owner,
            jxx::lang::jint index)
            : owner_(owner)
            , rootIterator_(owner->root_->listIterator(owner->offset_ + index))
            , cursor_(index)
            , lastReturned_(-1)
            , expectedRootSize_(owner->expectedRootSize_) {

            owner_->rangeCheckForAdd_(index);
        }

        jxx::lang::jbool hasNext() override {
            return cursor_ < owner_->size_;
        }

        jxx::Ptr<E> next() override {
            checkForComodification_();
            if (!hasNext()) {
                throw NoSuchElementException();
            }
            const auto value = rootIterator_->next();
            lastReturned_ = cursor_;
            ++cursor_;
            return value;
        }

        jxx::lang::jbool hasPrevious() override {
            return cursor_ > 0;
        }

        jxx::Ptr<E> previous() override {
            checkForComodification_();
            if (!hasPrevious()) {
                throw NoSuchElementException();
            }
            const auto value = rootIterator_->previous();
            --cursor_;
            lastReturned_ = cursor_;
            return value;
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

            rootIterator_->remove();
            if (lastReturned_ < cursor_) {
                --cursor_;
            }
            lastReturned_ = -1;
            owner_->updateAfterStructuralChange_(-1);
            expectedRootSize_ = owner_->expectedRootSize_;
        }

        void set(const jxx::Ptr<E>& element) override {
            checkForComodification_();
            if (lastReturned_ < 0) {
                throw jxx::lang::IllegalStateException();
            }
            rootIterator_->set(element);
        }

        void add(const jxx::Ptr<E>& element) override {
            checkForComodification_();
            rootIterator_->add(element);
            ++cursor_;
            lastReturned_ = -1;
            owner_->updateAfterStructuralChange_(1);
            expectedRootSize_ = owner_->expectedRootSize_;
        }
    };

public:
    SubList(
        const jxx::Ptr<List<E>>& root,
        jxx::lang::jint fromIndex,
        jxx::lang::jint toIndex)
        : root_(root)
        , offset_(fromIndex)
        , size_(toIndex - fromIndex)
        , expectedRootSize_(root == nullptr ? 0 : root->size()) {

        if (root_ == nullptr) {
            throw jxx::lang::NullPointerException();
        }
        if (fromIndex < 0 || toIndex < fromIndex ||
            toIndex > expectedRootSize_) {
            throw jxx::lang::IndexOutOfBoundsException();
        }
    }

    ~SubList() override = default;

    jxx::lang::jint size() override {
        checkForComodification_();
        return size_;
    }

    jxx::lang::jbool isEmpty() override {
        return size() == 0;
    }

    jxx::Ptr<E> get(jxx::lang::jint index) const override {
        checkForComodification_();
        rangeCheck_(index);
        return root_->get(offset_ + index);
    }

    jxx::Ptr<E> set(
        jxx::lang::jint index,
        const jxx::Ptr<E>& element) override {

        checkForComodification_();
        rangeCheck_(index);
        return root_->set(offset_ + index, element);
    }

    void add(
        jxx::lang::jint index,
        const jxx::Ptr<E>& element) override {

        checkForComodification_();
        rangeCheckForAdd_(index);
        root_->add(offset_ + index, element);
        updateAfterStructuralChange_(1);
    }

    jxx::lang::jbool add(const jxx::Ptr<E>& element) override {
        add(size_, element);
        return true;
    }

    jxx::Ptr<E> remove(jxx::lang::jint index) override {
        checkForComodification_();
        rangeCheck_(index);
        const auto oldValue = root_->remove(offset_ + index);
        updateAfterStructuralChange_(-1);
        return oldValue;
    }

    jxx::lang::jbool contains(
        const jxx::Ptr<jxx::lang::Object>& object) override {

        return indexOf(object) >= 0;
    }

    jxx::lang::jbool remove(
        const jxx::Ptr<jxx::lang::Object>& object) override {

        const auto index = indexOf(object);
        if (index < 0) {
            return false;
        }
        remove(index);
        return true;
    }

    jxx::lang::jint indexOf(
        const jxx::Ptr<jxx::lang::Object>& object) override {

        checkForComodification_();
        for (jxx::lang::jint index = 0; index < size_; ++index) {
            const auto candidate =
                jxx::CAST<jxx::lang::Object>(root_->get(offset_ + index));
            if (equalsObject_(object, candidate)) {
                return index;
            }
        }
        return -1;
    }

    jxx::lang::jint lastIndexOf(
        const jxx::Ptr<jxx::lang::Object>& object) override {

        checkForComodification_();
        for (jxx::lang::jint index = size_ - 1; index >= 0; --index) {
            const auto candidate =
                jxx::CAST<jxx::lang::Object>(root_->get(offset_ + index));
            if (equalsObject_(object, candidate)) {
                return index;
            }
        }
        return -1;
    }

    jxx::lang::jbool addAll(
        const jxx::Ptr<wildcard::CollectionExtends<E>>& collection) override {

        return addAll(size_, collection);
    }

    jxx::lang::jbool addAll(
        jxx::lang::jint index,
        const jxx::Ptr<wildcard::CollectionExtends<E>>& collection) override {

        checkForComodification_();
        rangeCheckForAdd_(index);
        if (collection == nullptr) {
            throw jxx::lang::NullPointerException();
        }

        const auto count = collection->size();
        if (count == 0) {
            return false;
        }

        root_->addAll(offset_ + index, collection);
        updateAfterStructuralChange_(count);
        return true;
    }

    void clear() override {
        removeRange(0, size_);
    }

    jxx::Ptr<Iterator<E>> iterator() override {
        return jxx::CAST<Iterator<E>>(listIterator());
    }

    jxx::Ptr<ListIterator<E>> listIterator() override {
        return listIterator(0);
    }

    jxx::Ptr<ListIterator<E>> listIterator(
        jxx::lang::jint index) override {

        checkForComodification_();
        rangeCheckForAdd_(index);

        auto self =
            jxx::CAST<SubList<E>>(this->thisPtr());

        if (self == nullptr) {
            throw jxx::lang::IllegalStateException();
        }

        return jxx::CAST<ListIterator<E>>(
            jxx::NEW<SubListIterator>(self, index));
    }

    jxx::Ptr<List<E>> subList(
        jxx::lang::jint fromIndex,
        jxx::lang::jint toIndex) override {

        checkForComodification_();
        if (fromIndex < 0 || toIndex < fromIndex || toIndex > size_) {
            throw jxx::lang::IndexOutOfBoundsException();
        }

        auto view = jxx::NEW<SubList<E>>(
            root_,
            offset_ + fromIndex,
            offset_ + toIndex);

        return jxx::CAST<List<E>>(view);
    }

    jxx::lang::ObjectArray toArray() override {
        checkForComodification_();
        auto array = jxx::NEW<jxx::lang::ObjectArrayType>(
            static_cast<std::uint32_t>(size_));

        for (jxx::lang::jint index = 0; index < size_; ++index) {
            (*array)[index] = jxx::CAST<jxx::lang::Object>(
                root_->get(offset_ + index));
        }
        return array;
    }

protected:
    void removeRange(
        jxx::lang::jint fromIndex,
        jxx::lang::jint toIndex) {

        checkForComodification_();
        if (fromIndex < 0 || toIndex < fromIndex || toIndex > size_) {
            throw jxx::lang::IndexOutOfBoundsException();
        }

        const auto count = toIndex - fromIndex;
        for (jxx::lang::jint index = 0; index < count; ++index) {
            root_->remove(offset_ + fromIndex);
        }
        if (count != 0) {
            updateAfterStructuralChange_(-count);
        }
    }
};

} // namespace jxx::util
