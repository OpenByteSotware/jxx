#pragma once

#include "lang/jxx_types.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.Iterator.h"
#include "util/jxx.util.Spliterator.h"
#include "function/jxx.util.function.Consumer.h"

namespace jxx::util {

/**
 * Java 8 parity: java.util.Spliterators
 *
 * Minimal implementation to support:
 *   Spliterators.spliteratorUnknownSize(Iterator<? extends T> it, int characteristics)
 */
class Spliterators final {
public:
    Spliterators() = delete;

    template <class T>
    class IteratorSpliterator final : public jxx::lang::Object, public Spliterator<T> {
    public:
        IteratorSpliterator(jxx::Ptr<Iterator<T>> it, jxx::lang::jint characteristics)
            : it_(std::move(it)), characteristics_(characteristics) {}

        jxx::lang::jbool tryAdvance(jxx::Ptr<jxx::util::function::Consumer<T>> action) override {
            if (!action) {
                throw jxx::lang::NullPointerException(jxx::NEW<jxx::lang::String>("action"));
            }
            if (!it_) return false;
            if (!it_->hasNext()) return false;
            action->accept(it_->next());
            return true;
        }

        jxx::lang::jlong estimateSize() const override {
            // Unknown size in Java => Long.MAX_VALUE
            return (jxx::lang::jlong)0x7fffffffffffffffLL;
        }

        jxx::lang::jint characteristics() const override {
            return characteristics_;
        }

        jxx::Ptr<Spliterator<T>> trySplit() override {
            // Unknown-size iterator spliterator is generally not splittable
            return nullptr;
        }

    private:
        jxx::Ptr<Iterator<T>> it_;
        jxx::lang::jint characteristics_;
    };

    template <class T>
    static jxx::Ptr<Spliterator<T>> spliteratorUnknownSize(jxx::Ptr<Iterator<T>> const it, jxx::lang::jint characteristics) {
        if (!it) {
            throw jxx::lang::NullPointerException(jxx::NEW<jxx::lang::String>("it"));
        }
        return jxx::NEW<IteratorSpliterator<T>>(std::move(it), characteristics);
    }
};

} // namespace jxx::util