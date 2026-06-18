#pragma once

#include "lang/jxx.lang.Object.h"

namespace jxx {
namespace util {

	// Marker interface for random access collections (e.g., ArrayList)
class RandomAccess {
public:
    virtual ~RandomAccess() = default;
};

} // namespace util
} // namespace jxx
