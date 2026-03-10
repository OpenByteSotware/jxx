
#pragma once
#include <functional>
#include "lang/jxx.lang.h"

namespace jxx { namespace util {

template <typename T>
class Iterator {
public:
    virtual ~Iterator() = default;
    virtual bool hasNext() = 0;
    virtual T next() = 0;
    virtual void remove() { JXX_THROW(jxx::lang::UnsupportedOperationException, "remove"); }
};

}} // namespace jxx::util
