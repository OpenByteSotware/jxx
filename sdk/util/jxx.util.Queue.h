#pragma once
#include "lang/jxx.lang.Object.h"
#include "util/jxx.util.Collection.h"

namespace jxx {
namespace util {

template <typename E>
class Queue : virtual public Collection<E> {
public:
    virtual ~Queue() = default;

    virtual jxx::lang::jbool add(const jxx::Ptr<E>& e) override {
        if (offer(e)) return true;
        throw IllegalStateException();
    }

    virtual jxx::lang::jbool offer(const jxx::Ptr<E> e) = 0;
    virtual jxx::Ptr<E> remove() = 0;
    virtual jxx::Ptr<E> poll() = 0;
    virtual jxx::Ptr<E> element() = 0;
    virtual jxx::Ptr<E> peek() = 0;
};

} // namespace util
} // namespace jxx
