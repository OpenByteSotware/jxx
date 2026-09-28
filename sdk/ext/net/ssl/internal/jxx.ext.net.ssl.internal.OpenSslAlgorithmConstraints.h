#pragma once

#include "security/jxx.security.AlgorithmConstraints.h"

namespace jxx::ext::net::ssl::internal {

::jxx::lang::jbool permitsAlgorithm(
    const ::jxx::Ptr<::jxx::security::AlgorithmConstraints>& constraints,
    const ::jxx::Ptr<::jxx::security::CryptoPrimitive>& primitive,
    const ::jxx::Ptr<::jxx::lang::String>& algorithm);

} // namespace jxx::ext::net::ssl::internal
