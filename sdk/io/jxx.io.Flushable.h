#pragma once
#include "lang/jxx.lang.ClassInfo.h"
namespace jxx::io {
class Flushable : public ::jxx::lang::InterfaceBase<Flushable> {
public:
    ~Flushable() override = default;
    virtual void flush() = 0;
};
} // namespace jxx::io
