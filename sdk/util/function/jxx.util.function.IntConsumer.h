#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"
namespace jxx::util::function {
class IntConsumer : public ::jxx::lang::InterfaceBase<IntConsumer> {
public:
    ~IntConsumer() override = default;
    virtual void accept(::jxx::lang::jint value) = 0;
};
} // namespace jxx::util::function
