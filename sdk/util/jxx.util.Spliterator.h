#pragma once

#include "lang/jxx.lang.Object.h"
#include "util/function/jxx.util.function.Consumer.h"

namespace jxx {
namespace util {

template <typename T>
class Spliterator  {
public:
    static constexpr jxx::lang::jint ORDERED    = 0x00000010;
    static constexpr jxx::lang::jint DISTINCT   = 0x00000001;
    static constexpr jxx::lang::jint SORTED     = 0x00000004;
    static constexpr jxx::lang::jint SIZED      = 0x00000040;
    static constexpr jxx::lang::jint NONNULL    = 0x00000100;
    static constexpr jxx::lang::jint IMMUTABLE  = 0x00000400;
    static constexpr jxx::lang::jint CONCURRENT = 0x00001000;
    static constexpr jxx::lang::jint SUBSIZED   = 0x00004000;

    virtual ~Spliterator() = default;
    virtual jxx::lang::jbool tryAdvance(jxx::Ptr<jxx::util::function::Consumer<T>> action) = 0;
    virtual void forEachRemaining(jxx::Ptr<jxx::util::function::Consumer<T>> action) = 0;
    virtual jxx::Ptr<Spliterator<T>> trySplit() = 0;
    virtual jxx::lang::jlong estimateSize() const = 0;
    virtual jxx::lang::jint characteristics() const = 0;
};

} // namespace util
} // namespace jxx
