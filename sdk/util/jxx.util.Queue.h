#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "util/jxx.util.Collection.h"

namespace jxx::util {

template <typename E>
class Queue
    : public ::jxx::lang::InterfaceBase<Queue<E>, Collection<E>> {
public:
    ~Queue() override = default;

    virtual ::jxx::lang::jbool offer(
        const ::jxx::Ptr<E>& element) = 0;
    virtual ::jxx::Ptr<E> remove() = 0;
    virtual ::jxx::Ptr<E> poll() = 0;
    virtual ::jxx::Ptr<E> element() = 0;
    virtual ::jxx::Ptr<E> peek() = 0;
};

} // namespace jxx::util
