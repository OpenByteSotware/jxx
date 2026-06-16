#pragma once

#include "lang/jxx.lang.Object.h"

namespace jxx {
namespace util {
namespace function {

template <typename T>
class Predicate {
public:
    virtual ~Predicate() = default;
    virtual jbool test(jxx::Ptr<T> value) = 0;
};

} // namespace function
} // namespace util
} // namespace jxx
