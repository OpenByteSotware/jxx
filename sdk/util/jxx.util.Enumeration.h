#pragma once

#include "lang/jxx.lang.Object.h"

namespace jxx {
namespace util {

template <typename E>
class Enumeration  {
public:
    virtual ~Enumeration() = default;
    virtual jxx::lang::jbool hasMoreElements() = 0;
    virtual jxx::Ptr<E> nextElement() = 0;
};

} // namespace util
} // namespace jxx
