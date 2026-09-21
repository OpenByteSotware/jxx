#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"
namespace jxx::util::function {
class DoubleConsumer : public ::jxx::lang::InterfaceBase<DoubleConsumer> {
public:
    ~DoubleConsumer() override = default;
    virtual void accept(::jxx::lang::jdouble value) = 0;
};
} // namespace jxx::util::function
