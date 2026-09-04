#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"
namespace jxx::lang { template <typename T> class Comparable : public jxx::lang::InterfaceBase<Comparable<T>> { public: virtual ~Comparable() = default; virtual jint compareTo(const jxx::Ptr<T>& other) const = 0; }; }
