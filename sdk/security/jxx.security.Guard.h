#pragma once

#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::lang { class Object; }
namespace jxx::security {

class Guard
    : public ::jxx::lang::InterfaceBase<Guard> {
public:
    ~Guard() override = default;
    virtual void checkGuard(
        const ::jxx::Ptr<::jxx::lang::Object>& object) = 0;
};

} // namespace jxx::security
