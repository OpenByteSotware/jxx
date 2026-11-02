#pragma once
#include "lang/jxx.lang.AutoCloseable.h"
#include "lang/jxx.lang.ClassInfo.h"
namespace jxx::io {
class Closeable : public ::jxx::lang::InterfaceBase<Closeable, ::jxx::lang::AutoCloseable> {
public:
    ~Closeable() override = default;
};
} // namespace jxx::io
