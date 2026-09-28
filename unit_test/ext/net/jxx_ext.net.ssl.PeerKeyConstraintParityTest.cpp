#include <gtest/gtest.h>
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslPublicKey.h"
namespace {
TEST(PeerKeyConstraintParityTest, PublicKeyUsesX509Encoding) {
    const auto encoded = ::jxx::NEW<
        ::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>(1);
    (*encoded)[0] = 7;
    const auto key = ::jxx::NEW<
        ::jxx::ext::net::ssl::internal::OpenSslPublicKey>(
            ::jxx::NEW<::jxx::lang::String>("RSA"), encoded);
    EXPECT_EQ(key->getAlgorithm()->utf8(), "RSA");
    EXPECT_EQ(key->getFormat()->utf8(), "X.509");
    EXPECT_NE(key->getEncoded(), encoded);
}
}
