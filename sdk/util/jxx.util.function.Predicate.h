#pragma once

#include "io/jxx.lang.Object.h"

namespace jxx {
template <typename T> class Ptr;
}

namespace jxx {
namespace util {
namespace function {

template <typename T>
class Predicate : virtual public jxx::lang::Object {
public:
    virtual ~Predicate() = default;
    virtual jbool test(jxx::Ptr<T> value) = 0;
};

} // namespace function
} // namespace util
} // namespace jxx
