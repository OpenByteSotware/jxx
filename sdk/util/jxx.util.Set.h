#pragma once

#include "jxx.util.Collection.h"

namespace jxx::util {

template <class E>
struct Set : public virtual Collection<E> {
    virtual ~Set() = default;
};

} // namespace jxx::util
