#pragma once
#include <type_traits>

#include "jxx_types.h"
#include "jxx.lang.NullPointerException.h"
#include "jxx.lang.String.h"

#include "util/jxx.util.Iterator.h"
#include "util/jxx.util.Spliterator.h"
#include "util/jxx.util.Spliterators.h"
#include "util/function/jxx.util.function.Consumer.h"


namespace jxx::lang {

    /**
     * Java 8 parity: java.lang.Iterable<T>
     *
     * Methods:
     *   Iterator<T> iterator();
     *   default void forEach(Consumer<? super T> action);
     *   default Spliterator<T> spliterator();
     *
     * Constraints:
     * - Interface does NOT inherit jxx::lang::Object (Java-like).
     */
    template <class T>
    struct Iterable {
        virtual ~Iterable() = default;

        // Java: Iterator<T> iterator()
        virtual jxx::Ptr<jxx::util::Iterator<T>> iterator() = 0;

        // Java 8 default: void forEach(Consumer<? super T> action)
        virtual void forEach(jxx::Ptr<jxx::util::function::Consumer<T>> action) {
            if (!action) {
                throw jxx::lang::NullPointerException(JXX_NEW<jxx::lang::String>("action"));
            }
            auto it = iterator();
            if (!it) {
                // Not specified by Java, but avoids UB; treat as NPE-ish in practice.
                throw jxx::lang::NullPointerException(JXX_NEW<jxx::lang::String>("iterator"));
            }
            while (it->hasNext()) {
                action->accept(it->next());
            }
        }

        // Variance-friendly forEach: Consumer<U> where T -> U convertible (Consumer<? super T>)
        template <
            class U,
            typename std::enable_if_t<
            !std::is_same_v<T, U> &&
            (std::is_convertible_v<T, U> || std::is_constructible_v<U, T>),
            int
            > = 0
        >
        void forEach(jxx::Ptr<jxx::util::function::Consumer<U>> action) {
            if (!action) {
                throw jxx::lang::NullPointerException(JXX_NEW<jxx::lang::String>("action"));
            }

            // Wrap Consumer<U> into Consumer<T> (Java '? super T' behavior).
            // Important: avoid static_cast for jxx::Ptr<Derived> -> jxx::Ptr<Base>; let implicit conversion work.
            auto adapter = jxx::util::function::consumerOf<T>(
                [action](T t) {
                    action->accept(t); // implicit T -> U conversion occurs here if needed
                }
            );

            forEach(adapter);
        }

        // Java 8 default: Spliterator<T> spliterator()
        virtual jxx::Ptr<jxx::util::Spliterator<T>> spliterator() {
            // Java 8 uses Spliterators.spliteratorUnknownSize(iterator(), 0)
            return jxx::util::Spliterators::spliteratorUnknownSize<T>(iterator(), 0);
        }
    };

} // namespace jxx::lang