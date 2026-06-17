#pragma once

#include "io/jxx.lang.Object.h"

namespace jxx {
namespace util {

class RandomAccess : virtual public jxx::lang::Object {
public:
    virtual ~RandomAccess() = default;
};

} // namespace util
} // namespace jxx
