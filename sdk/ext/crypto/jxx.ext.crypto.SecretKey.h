#pragma once

#include "ext/security/auth/jxx.ext.security.auth.Destroyable.h"
#include "security/jxx.security.Key.h"

namespace jxx::ext::crypto {

class SecretKey
    : public ::jxx::lang::InterfaceBase<
          SecretKey,
          ::jxx::security::Key,
          ::jxx::ext::security::auth::Destroyable> {
public:
    ~SecretKey() override = default;
};

} // namespace jxx::ext::crypto
