#pragma once
#include "lang/jxx_types.h"
namespace jxx::lang { template <typename T> class Comparable { public: virtual ~Comparable() = default; virtual jint compareTo(const jxx::Ptr<T>& other) const = 0; }; }
