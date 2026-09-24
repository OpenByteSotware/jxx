#pragma once

#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::util::function {

template <typename T>
class ConsumerSuper
    : public ::jxx::lang::InterfaceBase<ConsumerSuper<T>> {
public:
    ~ConsumerSuper() override = default;

    virtual void acceptSuper(
        const ::jxx::Ptr<T>& value) = 0;
};

} // namespace jxx::util::function
