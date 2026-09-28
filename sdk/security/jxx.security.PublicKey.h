#pragma once

#include "security/jxx.security.Key.h"

namespace jxx::security {

class PublicKey
    : public ::jxx::lang::InterfaceBase<PublicKey, Key> {
public:
    ~PublicKey() override = default;
};

} // namespace jxx::security
