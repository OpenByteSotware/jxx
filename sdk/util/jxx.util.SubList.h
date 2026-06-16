#pragma once

#include "util/jxx.util.AbstractList.h"
#include "util/jxx.util.List.h"

namespace jxx {
template <typename T> class Ptr;
}

namespace jxx {
namespace util {

template <typename E>
class SubList : public AbstractList<E> {
private:
    jxx::Ptr<List<E>> root;
    jint offset_;
    jint size_;

public:
    SubList(jxx::Ptr<List<E>> list, jint fromIndex, jint toIndex)
        : root(list), offset_(fromIndex), size_(toIndex - fromIndex) {
        if (fromIndex < 0 || toIndex < fromIndex) {
            throw IndexOutOfBoundsException();
        }
    }

    virtual ~SubList() = default;

    virtual jxx::Ptr<E> get(jint index) override {
        rangeCheck(index);
        return root->get(offset_ + index);
    }

    virtual jxx::Ptr<E> set(jint index, jxx::Ptr<E> element) override {
        rangeCheck(index);
        return root->set(offset_ + index, element);
    }

    virtual void add(jint index, jxx::Ptr<E> element) override {
        rangeCheckForAddLocal(index);
        root->add(offset_ + index, element);
        ++size_;
        ++this->modCount;
    }

    virtual jxx::Ptr<E> remove(jint index) override {
        rangeCheck(index);
        jxx::Ptr<E> oldValue = root->remove(offset_ + index);
        --size_;
        ++this->modCount;
        return oldValue;
    }

    virtual jint size() override {
        return size_;
    }

    virtual jbool addAll(jint index, jxx::Ptr<Collection<E>> c) override {
        rangeCheckForAddLocal(index);
        const jint cSize = c->size();
        if (cSize == 0) {
            return false;
        }
        root->addAll(offset_ + index, c);
        size_ += cSize;
        ++this->modCount;
        return true;
    }

    virtual jxx::Ptr<List<E>> subList(jint fromIndex, jint toIndex) override {
        if (fromIndex < 0 || toIndex > size_ || fromIndex > toIndex) {
            throw IndexOutOfBoundsException();
        }
        return jxx::Ptr<List<E>>(new SubList<E>(root, offset_ + fromIndex, offset_ + toIndex));
    }

protected:
    virtual void removeRange(jint fromIndex, jint toIndex) override {
        if (fromIndex < 0 || toIndex > size_ || fromIndex > toIndex) {
            throw IndexOutOfBoundsException();
        }
        for (jint i = 0; i < (toIndex - fromIndex); ++i) {
            root->remove(offset_ + fromIndex);
        }
        size_ -= (toIndex - fromIndex);
        ++this->modCount;
    }

private:
    void rangeCheck(jint index) {
        if (index < 0 || index >= size_) {
            throw IndexOutOfBoundsException();
        }
    }

    void rangeCheckForAddLocal(jint index) {
        if (index < 0 || index > size_) {
            throw IndexOutOfBoundsException();
        }
    }
};

} // namespace util
} // namespace jxx
