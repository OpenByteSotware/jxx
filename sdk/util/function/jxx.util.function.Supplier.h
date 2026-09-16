#pragma once

#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::util::function {
template <typename T>
class Supplier : public ::jxx::lang::InterfaceBase<Supplier<T>> {
public:
    ~Supplier() override = default;
    virtual ::jxx::Ptr<T> get() = 0;
};
} // namespace jxx::util::function
