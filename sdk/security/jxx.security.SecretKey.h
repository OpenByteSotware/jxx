#pragma once
#include "security/jxx.security.Key.h"
namespace jxx::security {
class SecretKey : public ::jxx::lang::InterfaceBase<SecretKey, Key> {
public:
    ~SecretKey() override = default;
};
} // namespace jxx::security
