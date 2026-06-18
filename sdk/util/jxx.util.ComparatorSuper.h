#pragma once

#include "lang/jxx.lang.Object.h"

namespace jxx {
namespace util {

template <typename T>
class ComparatorSuper : virtual public jxx::lang::Object {
public:
    virtual ~ComparatorSuper() = default;
    virtual jxx::lang::jint compare(jxx::Ptr<T> a, jxx::Ptr<T> b) = 0;
};

} // namespace util
} // namespace jxx
