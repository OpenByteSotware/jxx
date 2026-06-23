#pragma once

#include "util/jxx.util.AbstractList.h"
#include "util/jxx.util.List.h"
#include "util/jxx.util.RandomAccess.h"
#include "util/jxx.util.ListIterator.h"
#include "util/jxx.util.ConcurrentModificationException.h"
#include "lang/jxx.lang.NoSuchElementException.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace jxx {
namespace util {

template <typename E>
class SubList : public AbstractList<E>, public virtual RandomAccess {
private:
    jxx::Ptr<List<E>> root;
    jxx::lang::jint offset_;
    jxx::lang::jint size_;

    // Inner ListIterator implementation for SubList
    class SubListIterator : public virtual ListIterator<E> {
    private:
        SubList<E>* subList_;
        jxx::Ptr<ListIterator<E>> rootIterator_;
        jxx::lang::jint cursor_;
        jxx::lang::jint lastRet_;
        jxx::lang::jint expectedModCount_;

    public:
        SubListIterator(SubList<E>* subList, jxx::lang::jint index)
            : subList_(subList), cursor_(index), lastRet_(-1), 
              expectedModCount_(subList->modCount) {
            rootIterator_ = subList->root->listIterator(subList->offset_ + index);
        }

        virtual jxx::lang::jbool hasNext() override {
            return cursor_ < subList_->size_;
        }

        virtual jxx::Ptr<E> next() override {
            if (expectedModCount_ != subList_->modCount) {
                throw jxx::util::ConcurrentModificationException();
            }
            if (!hasNext()) {
                throw jxx::lang::NoSuchElementException();
            }
            lastRet_ = cursor_++;
            return rootIterator_->next();
        }

        virtual jxx::lang::jbool hasPrevious() override {
            return cursor_ > 0;
        }

        virtual jxx::Ptr<E> previous() override {
            if (expectedModCount_ != subList_->modCount) {
                throw jxx::util::ConcurrentModificationException();
            }
            if (!hasPrevious()) {
                throw jxx::lang::NoSuchElementException();
            }
            cursor_--;
            lastRet_ = cursor_;
            return rootIterator_->previous();
        }

        virtual jxx::lang::jint nextIndex() override {
            return cursor_;
        }

        virtual jxx::lang::jint previousIndex() override {
            return cursor_ - 1;
        }

        virtual void remove() override {
            if (lastRet_ < 0) {
                throw jxx::lang::IllegalStateException();
            }
            if (expectedModCount_ != subList_->modCount) {
                throw jxx::util::ConcurrentModificationException();
            }
            rootIterator_->remove();
            if (lastRet_ < cursor_) {
                cursor_--;
            }
            lastRet_ = -1;
            subList_->size_--;
            subList_->modCount++;
            expectedModCount_ = subList_->modCount;
        }

        virtual void set(jxx::Ptr<E> e) override {
            if (lastRet_ < 0) {
                throw jxx::lang::IllegalStateException();
            }
            if (expectedModCount_ != subList_->modCount) {
                throw jxx::util::ConcurrentModificationException();
            }
            rootIterator_->set(e);
        }

        virtual void add(jxx::Ptr<E> e) override {
            if (expectedModCount_ != subList_->modCount) {
                throw jxx::util::ConcurrentModificationException();
            }
            rootIterator_->add(e);
            cursor_++;
            lastRet_ = -1;
            subList_->size_++;
            subList_->modCount++;
            expectedModCount_ = subList_->modCount;
        }
    };

public:
    SubList(jxx::Ptr<List<E>> list, jxx::lang::jint fromIndex, jxx::lang::jint toIndex)
        : root(list), offset_(fromIndex), size_(toIndex - fromIndex) {
        if (list == nullptr) {
            throw jxx::lang::NullPointerException();
        }
        if (fromIndex < 0 || toIndex < fromIndex) {
            throw jxx::lang::IndexOutOfBoundsException();
        }
    }

    virtual ~SubList() = default;
    
    virtual void replaceAll(jxx::Ptr<function::UnaryOperator<E>> op) override {
    }

    
    virtual void sort(jxx::Ptr<ComparatorSuper<E>> c) override {

    }

    virtual jxx::Ptr<Spliterator<E>> spliterator() override {
        return nullptr;
    }

    virtual jxx::Ptr<E> get(jxx::lang::jint index) override {
        rangeCheck(index);
        return root->get(offset_ + index);
    }

    virtual jxx::Ptr<E> set(jxx::lang::jint index, jxx::Ptr<E> element) override {
        rangeCheck(index);
        return root->set(offset_ + index, element);
    }

    virtual void add(jxx::lang::jint index, jxx::Ptr<E> element) override {
        rangeCheckForAddLocal(index);
        root->add(offset_ + index, element);
        ++size_;
        ++this->modCount;
    }

    virtual jxx::Ptr<E> remove(jxx::lang::jint index) override {
        rangeCheck(index);
        jxx::Ptr<E> oldValue = root->remove(offset_ + index);
        --size_;
        ++this->modCount;
        return oldValue;
    }

    virtual jxx::lang::jint size() override {
        return size_;
    }

    virtual jxx::lang::jbool addAll(jxx::lang::jint index, jxx::Ptr<wildcard::CollectionExtends<E>> c) override {
        rangeCheckForAddLocal(index);
        if (c == nullptr) {
            throw jxx::lang::NullPointerException();
        }
        const jxx::lang::jint cSize = c->size();
        if (cSize == 0) {
            return false;
        }
        root->addAll(offset_ + index, c);
        size_ += cSize;
        ++this->modCount;
        return true;
    }

    virtual jxx::Ptr<List<E>> subList(jxx::lang::jint fromIndex, jxx::lang::jint toIndex) override {
        if (fromIndex < 0 || toIndex > size_ || fromIndex > toIndex) {
            throw jxx::lang::IndexOutOfBoundsException();
        }
        return jxx::Ptr<List<E>>(jxx::NEW<SubList<E>>(root, offset_ + fromIndex, offset_ + toIndex));
    }

    virtual jxx::Ptr<jxx::util::Iterator<E>> iterator() override {
        return listIterator(0);
    }

    virtual jxx::Ptr<ListIterator<E>> listIterator(jxx::lang::jint index) override {
        if (index < 0 || index > size_) {
            throw jxx::lang::IndexOutOfBoundsException();
        }
        return jxx::Ptr<ListIterator<E>>(jxx::NEW<SubListIterator>(this, index));
    }

protected:
    virtual void removeRange(jxx::lang::jint fromIndex, jxx::lang::jint toIndex) override {
        if (fromIndex < 0 || toIndex > size_ || fromIndex > toIndex) {
            throw jxx::lang::IndexOutOfBoundsException();
        }
        for (jxx::lang::jint i = 0; i < (toIndex - fromIndex); ++i) {
            root->remove(offset_ + fromIndex);
        }
        size_ -= (toIndex - fromIndex);
        ++this->modCount;
    }

private:
    void rangeCheck(jxx::lang::jint index) {
        if (index < 0 || index >= size_) {
            throw jxx::lang::IndexOutOfBoundsException();
        }
    }

    void rangeCheckForAddLocal(jxx::lang::jint index) {
        if (index < 0 || index > size_) {
            throw jxx::lang::IndexOutOfBoundsException();
        }
    }
};

} // namespace util
} // namespace jxx
