#pragma once

#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::lang {

class Runnable
    : public jxx::lang::InterfaceBase<Runnable> {
public:
    ~Runnable() override = default;

    virtual void run() = 0;
};

} // namespace jxx::lang
