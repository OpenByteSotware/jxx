#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "util/jxx.util.Iterator.h"

namespace jxx::util {

template <typename E>
class ListIterator
    : public ::jxx::lang::InterfaceBase<ListIterator<E>, Iterator<E>> {
public:
    ~ListIterator() override = default;

    virtual ::jxx::lang::jbool hasPrevious() = 0;
    virtual ::jxx::Ptr<E> previous() = 0;
    virtual ::jxx::lang::jint nextIndex() = 0;
    virtual ::jxx::lang::jint previousIndex() = 0;
    virtual void set(const ::jxx::Ptr<E>& element) = 0;
    virtual void add(const ::jxx::Ptr<E>& element) = 0;
};

} // namespace jxx::util
