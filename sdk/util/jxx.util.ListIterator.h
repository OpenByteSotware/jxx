#pragma once

#include "util/jxx.util.Iterator.h"

namespace jxx {
namespace util {

template <typename E>
class ListIterator : virtual public Iterator<E> {
public:
    virtual ~ListIterator() = default;

    virtual jbool hasPrevious() = 0;
    virtual jxx::Ptr<E> previous() = 0;

    virtual jint nextIndex() = 0;
    virtual jint previousIndex() = 0;

    virtual void set(jxx::Ptr<E> e) = 0;
    virtual void add(jxx::Ptr<E> e) = 0;
};

} // namespace util
} // namespace jxx
