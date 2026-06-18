#pragma once

#include "lang/jxx.lang.Object.h"

namespace jxx {
namespace util {
namespace function {

template <typename T>
class ConsumerSuper {
public:
    virtual ~ConsumerSuper() = default;
    virtual void acceptSuper(jxx::Ptr<T> value) = 0;
};

} // namespace function
} // namespace util
} // namespace jxx
