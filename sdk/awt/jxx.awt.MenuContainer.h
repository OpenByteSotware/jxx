#pragma once

#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::awt {

class MenuContainer
    : public ::jxx::lang::InterfaceBase<MenuContainer> {
public:
    ~MenuContainer() override = default;
};

} // namespace jxx::awt
