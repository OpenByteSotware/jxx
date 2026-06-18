#pragma once

#include "lang/jxx.lang.Object.h"

namespace jxx {
namespace util {

template <typename E>
class Iterator {
public:
    virtual ~Iterator() = default;
    virtual jxx::lang::jbool hasNext() = 0;
    virtual jxx::Ptr<E> next() = 0;
    virtual void remove() = 0;
};

} // namespace util
} // namespace jxx
