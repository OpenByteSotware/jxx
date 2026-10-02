#include <gtest/gtest.h>

#include "security/jxx.security.KeyStore.h"

TEST(KeyStoreTrustedCertificatesTest, EmptyLoadedStoreReturnsEmptyArray) {
    const auto store = ::jxx::security::KeyStore::getInstance(
        ::jxx::NEW<::jxx::lang::String>("PKCS12"));
    store->load(nullptr, nullptr);

    const auto certificates = store->trustedCertificates();
    ASSERT_NE(nullptr, certificates);
    EXPECT_EQ(0, certificates->length);
}
