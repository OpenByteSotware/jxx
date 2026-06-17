#pragma once

#include "io/jxx.lang.Object.h"
#include "io/jxx.lang.Exceptions.h"
#include "io/jxx.util.function.ConsumerSuper.h"

namespace jxx {
template <typename T> class Ptr;
}

namespace jxx {
namespace util {
template <typename E> class Iterator;
template <typename E> class Spliterator;
}
}

namespace jxx {
namespace lang {

template <typename T>
class Iterable : virtual public Object {
public:
    virtual ~Iterable() = default;

    virtual jxx::Ptr<jxx::util::Iterator<T>> iterator() = 0;

    // Java 8 default method: forEach(Consumer<? super T>)
    virtual void forEach(jxx::Ptr<jxx::util::function::ConsumerSuper<T>> action) {
        if (action == nullptr) {
            throw NullPointerException();
        }
        auto it = iterator();
        while (it->hasNext()) {
            action->accept(it->next());
        }
    }

    // Java 8 default method
    virtual jxx::Ptr<jxx::util::Spliterator<T>> spliterator() {
        throw UnsupportedOperationException();
    }
};

} // namespace lang
} // namespace jxx
