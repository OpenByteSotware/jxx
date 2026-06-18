#pragma once
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "util/jxx.util.Spliterator.h"
#include "util/jxx.util.Iterator.h"
#include "util/jxx.util.spliterators.ArrayRefSpliterator.h"

namespace jxx {
namespace util {
namespace spliterators {

template <typename E>
class IteratorSpliterator : public virtual jxx::util::Spliterator<E> {
private:
    static constexpr jxx::lang::jint BATCH_UNIT = 64;
    static constexpr jxx::lang::jint MAX_BATCH = 1 << 10;

    jxx::Ptr<jxx::util::Iterator<E>> iterator_;
    jxx::lang::jlong estimate_;
    jxx::lang::jint characteristics_;
    jxx::lang::jint batch_;
    jxx::Ptr<jxx::util::ComparatorSuper<E>> comparator_;

public:
    IteratorSpliterator(
        jxx::Ptr<jxx::util::Iterator<E>> iterator,
        jxx::lang::jlong estimatedSize,
        jxx::lang::jint characteristics,
        jxx::Ptr<jxx::util::ComparatorSuper<E>> comparator = nullptr)
        : iterator_(iterator)
        , estimate_(estimatedSize)
        , characteristics_(characteristics)
        , batch_(0)
        , comparator_(comparator) {
        if (iterator_ == nullptr) {
            throw jxx::lang::NullPointerException();
        }
    }

    virtual ~IteratorSpliterator() = default;

    virtual jxx::lang::jbool tryAdvance(jxx::Ptr<jxx::util::function::Consumer<E>> action) override {
        if (action == nullptr) {
            throw jxx::lang::NullPointerException();
        }
        if (!iterator_->hasNext()) {
            return false;
        }
        action->accept(iterator_->next());
        if (estimate_ > 0) {
            --estimate_;
        }
        return true;
    }

    virtual void forEachRemaining(jxx::Ptr<jxx::util::function::Consumer<E>> action) override {
        if (action == nullptr) {
            throw jxx::lang::NullPointerException();
        }
        while (iterator_->hasNext()) {
            action->accept(iterator_->next());
            if (estimate_ > 0) {
                --estimate_;
            }
        }
    }

    virtual jxx::Ptr<jxx::util::Spliterator<E>> trySplit() override {
        if (!iterator_->hasNext()) {
            return nullptr;
        }

        jxx::lang::jint n = batch_ + BATCH_UNIT;
        if (n > MAX_BATCH) {
            n = MAX_BATCH;
        }
        if (estimate_ > 0 && static_cast<jxx::lang::jlong>(n) > estimate_) {
            n = static_cast<jxx::lang::jint>(estimate_);
        }
        if (n <= 0) {
            return nullptr;
        }

        auto a = jxx::NEW<jxx::lang::JxxArray<jxx::Ptr<E>, 1U>>(n);
        jxx::lang::jint j = 0;
        while (j < n && iterator_->hasNext()) {
            (*a)[j] = iterator_->next();
            j++;
        }
        if (j == 0) {
            return nullptr;
        }
        batch_ = j;
        if (estimate_ > 0) {
            estimate_ -= j;
        }

        return jxx::Ptr<jxx::util::Spliterator<E>>(jxx::NEW<ArrayRefSpliterator<E>>(a, (jxx::lang::jint)0, j, characteristics_, comparator_));
    }

    virtual jxx::lang::jlong estimateSize() override {
        return estimate_;
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
