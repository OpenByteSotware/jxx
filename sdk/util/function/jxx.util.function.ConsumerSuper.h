#pragma once

#include "lang/jxx.lang.Object.h"

namespace jxx {
namespace util {
namespace function {

template <typename T>
class ConsumerSuper : virtual public jxx::lang::Object {
public:
    virtual ~ConsumerSuper() = default;
    virtual void accept(jxx::Ptr<T> value) = 0;
};

} // namespace function
} // namespace util
} // namespace jxx
