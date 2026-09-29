#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"

namespace jxx::ext::security::auth {

class Destroyable
    : public ::jxx::lang::InterfaceBase<Destroyable> {
public:
    ~Destroyable() override = default;

    virtual void destroy() {
    }

    virtual ::jxx::lang::jbool
    isDestroyed() const {
        return false;
    }
};

} // namespace jxx::ext::security::auth
