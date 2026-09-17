#pragma once

#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::lang {

class AutoCloseable
    : public ::jxx::lang::InterfaceBase<AutoCloseable> {
public:
    ~AutoCloseable() override = default;

    virtual void close() = 0;
};

} // namespace jxx::lang
