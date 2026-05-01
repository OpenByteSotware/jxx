#pragma once

#include "jxx_types.h"
#include <stdexcept>

namespace jxx::util {

template <class E>
struct Iterator {
    virtual ~Iterator() = default;

    virtual jbool hasNext() = 0;
    virtual E next() = 0;

    virtual void remove() {
        throw std::logic_error("UnsupportedOperationException: Iterator.remove()");
    }
};

} // namespace jxx::util
