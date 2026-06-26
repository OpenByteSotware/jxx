#pragma once

#include "lang/jxx.lang.Object.h"
#include "util/jxx.util.Spliterator.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.IndexOutOfBoundsException.h"

namespace jxx {
namespace util {
namespace spliterators {

template <typename E>
class ArrayRefSpliterator : public virtual jxx::util::Spliterator<E> {
private:
    jxx::Ptr<jxx::JxxArray<jxx::Ptr<E>, 1U>> array_;
    jxx::lang::jint index_;
    jxx::lang::jint fence_;
    jxx::lang::jint characteristics_;
    jxx::Ptr<jxx::util::ComparatorSuper<E>> comparator_;

public:
    ArrayRefSpliterator(
        jxx::Ptr<jxx::JxxArray<jxx::Ptr<E>, 1U>> array,
        jxx::lang::jint origin,
        jxx::lang::jint fence,
        jxx::lang::jint characteristics,
        jxx::Ptr<jxx::util::ComparatorSuper<E>> comparator = nullptr)
        : array_(array)
        , index_(origin)
        , fence_(fence)
        , characteristics_(characteristics | jxx::util::Spliterator<E>::SIZED | jxx::util::Spliterator<E>::SUBSIZED)
        , comparator_(comparator) {
        if (array_ == nullptr) {
            throw jxx::lang::NullPointerException();
        }
        if (origin < 0 || fence < origin || fence > array_->size()) {
            throw jxx::lang::IndexOutOfBoundsException();
        }
    }

    virtual ~ArrayRefSpliterator() = default;

    virtual jxx::lang::jbool tryAdvance(jxx::Ptr<jxx::util::function::Consumer<E>> action) override {
        if (action == nullptr) {
            throw jxx::lang::NullPointerException();
        }
        if (index_ < fence_) {
            action->accept(array_(index_));
            index_++;
            return true;
        }
        return false;
    }

    virtual void forEachRemaining(jxx::Ptr<jxx::util::function::Consumer<E>> action) override {
        if (action == nullptr) {
            throw jxx::lang::NullPointerException();
        }
        while (index_ < fence_) {
            action->accept((*array_)[index_]);
            index_++;
        }
    }

    virtual jxx::Ptr<jxx::util::Spliterator<E>> trySplit() override {
        const jxx::lang::jint lo = index_;
        const jxx::lang::jint mid = lo + ((fence_ - lo) >> 1);
        if (lo >= mid) {
            return nullptr;
        }
        index_ = mid;
        return jxx::Ptr<jxx::util::Spliterator<E>>(
            new ArrayRefSpliterator<E>(array_, lo, mid, characteristics_, comparator_));
    }

    virtual jxx::lang::jlong estimateSize() override {
        return static_cast<jxx::lang::jlong>(fence_ - index_);
    }

    virtual jxx::lang::jint characteristics() override {
        return characteristics_;
    }

    virtual jxx::Ptr<jxx::util::ComparatorSuper<E>> getComparator() override {
        if (!this->hasCharacteristics(jxx::util::Spliterator<E>::SORTED)) {
            throw jxx::lang::IllegalStateException();
        }
        return comparator_;
    }
};

} // namespace spliterators
} // namespace util
} // namespace jxx
