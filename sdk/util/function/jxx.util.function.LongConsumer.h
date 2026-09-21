#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"
namespace jxx::util::function {
class LongConsumer : public ::jxx::lang::InterfaceBase<LongConsumer> {
public:
    ~LongConsumer() override = default;
    virtual void accept(::jxx::lang::jlong value) = 0;
};
} // namespace jxx::util::function
