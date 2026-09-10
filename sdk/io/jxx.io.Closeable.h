#pragma once
#include "io/jxx.io.AutoCloseable.h"
#include "lang/jxx.lang.ClassInfo.h"
namespace jxx::io {
class Closeable : public ::jxx::lang::InterfaceBase<Closeable, AutoCloseable> {
public:
    ~Closeable() override = default;
};
} // namespace jxx::io
