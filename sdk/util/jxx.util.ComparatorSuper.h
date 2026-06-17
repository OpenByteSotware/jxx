#pragma once

#include "io/jxx.lang.Object.h"

namespace jxx {
template <typename T> class Ptr;
}

namespace jxx {
namespace util {

template <typename T>
class ComparatorSuper : virtual public jxx::lang::Object {
public:
    virtual ~ComparatorSuper() = default;
    virtual jint compare(jxx::Ptr<T> a, jxx::Ptr<T> b) = 0;
};

} // namespace util
} // namespace jxx
