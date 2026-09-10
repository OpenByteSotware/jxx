#pragma once
#include "lang/jxx.lang.ClassInfo.h"
namespace jxx::io {
class AutoCloseable : public ::jxx::lang::InterfaceBase<AutoCloseable> {
public:
    ~AutoCloseable() override = default;
    virtual void close() = 0;
};
} // namespace jxx::io
