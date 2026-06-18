#pragma once
#include <type_traits>

#include "lang/jxx_types.h"
#include "lang/jxx.lang.NullPointerException.h"
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
    class Iterable {
    public:
        virtual ~Iterable() = default;

        // Java: Iterator<T> iterator()
        virtual jxx::Ptr<jxx::util::Iterator<T>> iterator() = 0;

        // Java 8 default: void forEach(Consumer<? super T> action)
        virtual void forEach(jxx::Ptr<jxx::util::function::Consumer<T>> action) {
            if (!action) {
                throw jxx::lang::NullPointerException("action");
            }
            auto it = iterator();
            if (!it) {
                // Not specified by Java, but avoids UB; treat as NPE-ish in practice.
                throw jxx::lang::NullPointerException("iterator");
            }
            while (it->hasNext()) {
                action->accept(it->next());
            }
        }

        // Variance-friendly forEach: Consumer<U> where T -> U convertible (Consumer<? super T>)
        template <
            class U,
            std::enable_if_t<
                !std::is_same_v<T, U> &&
                (std::is_convertible_v<T, U> || std::is_constructible_v<U, T>),
                int
            > = 0
        >
        void forEach(jxx::Ptr<jxx::util::function::Consumer<U>> action) {
            if (!action) {
                throw jxx::lang::NullPointerException(jxx::NEW<jxx::lang::String>("action"));
            }

            // Create a Consumer<T> adapter that wraps Consumer<U>
            struct ConsumerAdapter : public jxx::util::function::Consumer<T> {
                jxx::Ptr<jxx::util::function::Consumer<U>> wrapped;
                
                explicit ConsumerAdapter(jxx::Ptr<jxx::util::function::Consumer<U>> w) : wrapped(w) {}
                
                void accept(T value) override {
                    // implicit T -> U conversion occurs here
                    wrapped->accept(static_cast<U>(value));
                }
            };

            auto adapter = jxx::NEW<ConsumerAdapter>(action);
            forEach(adapter);
        }

        // Java 8 default: Spliterator<T> spliterator()
        virtual jxx::Ptr<jxx::util::Spliterator<T>> spliterator() {
            // Java 8 uses Spliterators.spliteratorUnknownSize(iterator(), 0)
            return jxx::util::Spliterators::template spliteratorUnknownSize<T>(iterator(), 0);
        }
    };

} // namespace jxx::lang