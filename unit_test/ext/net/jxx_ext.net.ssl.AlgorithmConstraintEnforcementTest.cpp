#include <gtest/gtest.h>
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslAlgorithmConstraints.h"
namespace {
TEST(AlgorithmConstraintEnforcementTest, NullConstraintPermitsAlgorithm) {
    EXPECT_TRUE(::jxx::ext::net::ssl::internal::permitsAlgorithm(
        nullptr,
        ::jxx::security::CryptoPrimitive::BLOCK_CIPHER(),
        ::jxx::NEW<::jxx::lang::String>("TLS_AES_128_GCM_SHA256")));
}
}
