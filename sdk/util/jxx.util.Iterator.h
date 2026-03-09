
#pragma once
#include <functional>
#include "jxx.util.NoSuchElementException.h"
#include "jxx.lang.IllegalStateException.h"

namespace jxx { namespace util {

template <typename T>
class Iterator {
public:
    virtual ~Iterator() = default;
    virtual bool hasNext() = 0;
    virtual T next() = 0;
    virtual void remove() { throw jxx::lang::UnsupportedOperationException("remove"); }
};

}} // namespace jxx::util
