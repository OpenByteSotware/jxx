#pragma once

#include "jxx_types.h"
#include "jxx.util.Iterator.h"

namespace jxx::util {

template <class E>
struct Iterable {
    virtual ~Iterable() = default;
    virtual jxx::Ptr<Iterator<E>> iterator() = 0;
};

} // namespace jxx::util
