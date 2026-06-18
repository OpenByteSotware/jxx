#pragma once

#include "util/jxx.util.Spliterator.h"
#include "util/jxx.util.Iterator.h"
#include "util/jxx.util.MapEntry.h"
#include "util/jxx.util.ComparatorSuper.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.IllegalStateException.h"

namespace jxx {
namespace util {
namespace spliterators {

template <typename K, typename V>
class KeyIteratorSpliterator : public virtual jxx::util::Spliterator<K> {
private:
    jxx::Ptr<jxx::util::Iterator<jxx::util::MapEntry<K, V>>> iterator_;
    jxx::lang::jlong estimate_;
    jxx::lang::jint characteristics_;
    jxx::Ptr<jxx::util::ComparatorSuper<K>> comparator_;

public:
    KeyIteratorSpliterator(
        jxx::Ptr<jxx::util::Iterator<jxx::util::MapEntry<K, V>>> iterator,
        jxx::lang::jlong estimatedSize,
        jxx::lang::jint characteristics,
        jxx::Ptr<jxx::util::ComparatorSuper<K>> comparator = nullptr)
        : iterator_(iterator)
        , estimate_(estimatedSize)
        , characteristics_(characteristics)
        , comparator_(comparator) {
        if (iterator_ == nullptr) {
            throw jxx::lang::NullPointerException();
        }
    }

    virtual ~KeyIteratorSpliterator() = default;

    virtual jxx::lang::jbool tryAdvance(jxx::Ptr<jxx::util::function::Consumer<K>> action) override {
        if (action == nullptr) {
            throw jxx::lang::NullPointerException();
        }
        if (!iterator_->hasNext()) {
            return false;
        }
        action->accept(iterator_->next()->getKey());
        if (estimate_ > 0) {
            --estimate_;
        }
        return true;
    }

    virtual void forEachRemaining(jxx::Ptr<jxx::util::function::Consumer<K>> action) override {
        if (action == nullptr) {
            throw jxx::lang::NullPointerException();
        }
        while (iterator_->hasNext()) {
            action->accept(iterator_->next()->getKey());
            if (estimate_ > 0) {
                --estimate_;
            }
        }
    }

    virtual jxx::Ptr<jxx::util::Spliterator<K>> trySplit() override {
        return nullptr;
    }

    virtual jxx::lang::jlong estimateSize() override {
        return estimate_;
    }

    virtual jxx::lang::jint characteristics() override {
        return characteristics_;
    }

    virtual jxx::Ptr<jxx::util::ComparatorSuper<K>> getComparator() override {
        if (!this->hasCharacteristics(jxx::util::Spliterator<K>::SORTED)) {
            throw jxx::lang::IllegalStateException();
        }
        return comparator_;
    }
};

} // namespace spliterators
} // namespace util
} // namespace jxx
