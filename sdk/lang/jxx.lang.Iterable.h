#pragma once
#include "lang/jxx_types.h"
#include "lang/jxx.lang.ClassInfo.h"
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
    template <typename T>
    class Iterable : public jxx::lang::InterfaceBase<Iterable<T>> {
    public:
        virtual ~Iterable() = default;

        // Java: Iterator<T> iterator()
        virtual jxx::Ptr<jxx::util::Iterator<T>> iterator() = 0;

        // Java 8 default: void forEach(Consumer<? super T> action)
        virtual void forEach(const jxx::Ptr<jxx::util::function::Consumer<T>>& action) {
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

        // Java 8 default: Spliterator<T> spliterator()
        virtual jxx::Ptr<jxx::util::Spliterator<T>> spliterator() {
            // Java 8 uses Spliterators.spliteratorUnknownSize(iterator(), 0)
            return jxx::util::Spliterators::template spliteratorUnknownSize<T>(iterator(), 0);
        }
    };

} // namespace jxx::lang