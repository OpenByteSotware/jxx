#pragma once

#include "lang/jxx_types.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.UnsupportedOperationException.h"
#include "util/function/jxx.util.function.Consumer.h"

namespace jxx::util {

    class jxx::lang::String;

    /**
     * Java 8 parity: java.util.Iterator<E>
     *
     * Methods:
     *  - boolean hasNext()
     *  - E next()
     *  - default void remove()  (optional op; default throws UnsupportedOperationException)
     *  - default void forEachRemaining(Consumer<? super E> action)
     *
     * Notes on E:
     *  - In Java, E is a reference type. In JXX, you typically use E = jxx::Ptr<Foo>.
     *  - For primitives, E may be jint, etc. That’s OK.
     *  - This interface returns E by value (same as Java); if E is a Ptr, that’s still by value.
     */
    template <class E>
    struct Iterator {
        virtual ~Iterator() = default;

        // Java: boolean hasNext()
        virtual jxx::lang::jbool hasNext() = 0;

        // Java: E next()
        // Contract: should throw NoSuchElementException if no more elements.
        virtual E next() = 0;

        // Java: default void remove()
        // Contract: optional operation; default throws UnsupportedOperationException.
        virtual void remove() {
            throw jxx::lang::UnsupportedOperationException(
                JXX_NEW<jxx::lang::String>("remove")
            );
        }

        /**
         * Java 8: default void forEachRemaining(Consumer<? super E> action)
         *
         * Exact-type overload: Consumer<E>
         */
        virtual void forEachRemaining(jxx::Ptr<jxx::util::function::Consumer<E>> action) {
            if (!action) {
                throw jxx::lang::NullPointerException(JXX_NEW<jxx::lang::String>("action"));
            }
            while (hasNext()) {
                action->accept(next());
            }
        }

        /**
         * Variance-friendly overload to mimic Consumer<? super E>.
         *
         * Allows Consumer<U> when E is convertible to U (i.e., U is a “supertype” of E).
         * Example: E = jxx::Ptr<Derived>, U = jxx::Ptr<Base>
         */
        template <
            class U,
            typename std::enable_if_t<
            !std::is_same_v<E, U> &&
            (std::is_convertible_v<E, U> || std::is_constructible_v<U, E>),
            int
            > = 0
        >
        void forEachRemaining(jxx::Ptr<jxx::util::function::Consumer<U>> action) {
            if (!action) {
                throw jxx::lang::NullPointerException(JXX_NEW<jxx::lang::String>("action"));
            }
            while (hasNext()) {
                action->accept(static_cast<U>(next()));
            }
        }
    };

} // namespace jxx::util