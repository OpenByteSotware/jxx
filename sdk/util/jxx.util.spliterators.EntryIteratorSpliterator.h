#pragma once

#include "lang/jxx.lang.Object.h"
#include "util/jxx.util.Spliterator.h"
#include "util/jxx.util.Iterator.h"
#include "util/jxx.util.MapEntry.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace jxx {
namespace util {
namespace spliterators {

template <typename K, typename V>
class EntryIteratorSpliterator : public virtual jxx::util::Spliterator<jxx::util::MapEntry<K, V>> {
private:
    jxx::Ptr<jxx::util::Iterator<jxx::util::MapEntry<K, V>>> iterator_;
    jxx::lang::jlong estimate_;
    jxx::lang::jint characteristics_;

public:
    EntryIteratorSpliterator(
        jxx::Ptr<jxx::util::Iterator<jxx::util::MapEntry<K, V>>> iterator,
        jxx::lang::jlong estimatedSize,
        jxx::lang::jint characteristics)
        : iterator_(iterator)
        , estimate_(estimatedSize)
        , characteristics_(characteristics) {
        if (iterator_ == nullptr) {
            throw jxx::lang::NullPointerException();
        }
    }

    virtual ~EntryIteratorSpliterator() = default;

    virtual jxx::lang::jbool tryAdvance(jxx::Ptr<jxx::util::function::Consumer<jxx::util::MapEntry<K, V>>> action) override {
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

    virtual void forEachRemaining(jxx::Ptr<jxx::util::function::Consumer<jxx::util::MapEntry<K, V>>> action) override {
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

    virtual jxx::Ptr<jxx::util::Spliterator<jxx::util::MapEntry<K, V>>> trySplit() override {
        return nullptr;
    }

    virtual jxx::lang::jlong estimateSize() override {
        return estimate_;
    }

    virtual jxx::lang::jint characteristics() override {
        return characteristics_;
    }
};

} // namespace spliterators
} // namespace util
} // namespace jxx
