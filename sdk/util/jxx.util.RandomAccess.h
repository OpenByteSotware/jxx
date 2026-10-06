#pragma once

#include "lang/jxx.lang.ClassInfo.h"

#include "lang/jxx.lang.Object.h"

namespace jxx {
namespace util {

	// Marker interface for random access collections (e.g., ArrayList)
class RandomAccess : public ::jxx::lang::InterfaceBase<RandomAccess> {
public:
    virtual ~RandomAccess() = default;
};

} // namespace util
} // namespace jxx
