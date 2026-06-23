#pragma once

#include "io/jxx.util.Collection.h"
#include "io/jxx.lang.Exceptions.h"

namespace jxx { template <typename T> class Ptr; }

namespace jxx {
namespace util {

template <typename E>
class Queue : virtual public Collection<E> {
public:
    virtual ~Queue() = default;

    virtual jxx::lang::jbool add(jxx::Ptr<E> e) override {
        if (offer(e)) return true;
        throw IllegalStateException();
    }

    virtual jxx::lang::jbool offer(jxx::Ptr<E> e) = 0;
    virtual jxx::Ptr<E> remove() = 0;
    virtual jxx::Ptr<E> poll() = 0;
    virtual jxx::Ptr<E> element() = 0;
    virtual jxx::Ptr<E> peek() = 0;
};

} // namespace util
} // namespace jxx
