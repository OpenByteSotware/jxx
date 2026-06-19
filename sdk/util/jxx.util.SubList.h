#pragma once

#include "util/jxx.util.AbstractList.h"
#include "util/jxx.util.List.h"
#include "util/jxx.util.RandomAccess.h"

namespace jxx {
namespace util {

template <typename E>
class SubList : public AbstractList<E>, public virtual RandomAccess {
private:
    jxx::Ptr<List<E>> root;
    jxx::lang::jint offset_;
    jxx::lang::jint size_;

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
        return jxx::Ptr<List<E>>(new SubList<E>(root, offset_ + fromIndex, offset_ + toIndex));
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
