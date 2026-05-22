#pragma once
#include <cstdint>
#include <type_traits>

#include "lang/jxx_types.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "util/function/jxx.util.function.Consumer.h"

namespace jxx::util {

    class jxx::lang::String;
/**
 * Java 8 parity: java.util.Spliterator<T>
 *
 * Interface => does NOT inherit jxx::lang::Object.
 * Minimal subset for Iterable.spliterator() default behavior:
 *   - tryAdvance(Consumer<? super T>)
 *   - forEachRemaining(Consumer<? super T>) default
 *   - estimateSize()
 *   - characteristics()
 *   - trySplit() (may return null)
 */
template <class T>
struct Spliterator {
    virtual ~Spliterator() = default;

    // Java: boolean tryAdvance(Consumer<? super T> action)
    virtual jxx::lang::jbool tryAdvance(jxx::Ptr<jxx::util::function::Consumer<T>> action) = 0;

    // Java 8 default: void forEachRemaining(Consumer<? super T> action)
    virtual void forEachRemaining(jxx::Ptr<jxx::util::function::Consumer<T>> action) {
        if (!action) {
            throw jxx::lang::NullPointerException(jxx::NEW<jxx::lang::String>("action"));
        }
        while (tryAdvance(action)) { /* keep advancing */ }
    }

    // Variance-friendly overload: Consumer<U> where T -> U convertible (Consumer<? super T>)
    template <
        class U,
        typename std::enable_if_t<
            !std::is_same_v<T, U> &&
            (std::is_convertible_v<T, U> || std::is_constructible_v<U, T>),
            int
        > = 0
    >
    void forEachRemaining(jxx::Ptr<jxx::util::function::Consumer<U>> action) {
        if (!action) {
            throw jxx::lang::NullPointerException(jxx::NEW<jxx::lang::String>("action"));
        }
        // Adapt by wrapping U-consumer into a T-consumer using your variance-friendly Consumer
        auto adapter = jxx::util::function::consumerOf<T>(
            [action](T t) { action->accept(static_cast<U>(t)); }
        );
        forEachRemaining(adapter);
    }

    // Java: Spliterator<T> trySplit()
    // Minimal: allow returning nullptr to indicate not splittable.
    virtual jxx::Ptr<Spliterator<T>> trySplit() { return nullptr; }

    // Java: long estimateSize()
    virtual jxx::lang::jlong estimateSize() const = 0;

    // Java: int characteristics()
    virtual jxx::lang::jint characteristics() const = 0;

    // Common characteristics constants (Java 8)
    static constexpr jxx::lang::jint ORDERED   = 0x00000010;
    static constexpr jxx::lang::jint DISTINCT  = 0x00000001;
    static constexpr jxx::lang::jint SORTED    = 0x00000004;
    static constexpr jxx::lang::jint SIZED     = 0x00000040;
    static constexpr jxx::lang::jint NONNULL   = 0x00000100;
    static constexpr jxx::lang::jint IMMUTABLE = 0x00000400;
    static constexpr jxx::lang::jint CONCURRENT= 0x00001000;
    static constexpr jxx::lang::jint SUBSIZED  = 0x00004000;
};

} // namespace jxx::util