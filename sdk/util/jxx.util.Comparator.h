#pragma once

#include "lang/jxx.lang.Object.h"

namespace jxx {
namespace util {

template <typename T>
class Comparator : virtual public jxx::lang::Object {
public:
    virtual ~Comparator() = default;
    virtual jint compare(jxx::Ptr<T> a, jxx::Ptr<T> b) = 0;
};

} // namespace util
} // namespace jxx
