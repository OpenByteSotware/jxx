#include <gtest/gtest.h>

#include "ext/net/ssl/jxx.ext.net.ssl.KeyManagerFactory.h"
#include "ext/net/ssl/jxx.ext.net.ssl.TrustManagerFactory.h"

namespace {

TEST(ManagerFactoryDefaultParityTest, NullStoreInitializationReturnsManagers) {
    const auto keyManagerFactory =
        ::jxx::ext::net::ssl::KeyManagerFactory::getInstance(
            ::jxx::ext::net::ssl::KeyManagerFactory::getDefaultAlgorithm());

    const ::jxx::Ptr<::jxx::security::KeyStore> keyStore = nullptr;
    const ::jxx::Ptr<
        ::jxx::ext::net::ssl::KeyManagerFactory::CharArray> password = nullptr;

    keyManagerFactory->init(keyStore, password);

    const auto keyManagers = keyManagerFactory->getKeyManagers();
    ASSERT_NE(keyManagers, nullptr);
    EXPECT_EQ(keyManagers->length, 1);

    const auto trustManagerFactory =
        ::jxx::ext::net::ssl::TrustManagerFactory::getInstance(
            ::jxx::ext::net::ssl::TrustManagerFactory::getDefaultAlgorithm());

    const ::jxx::Ptr<::jxx::security::KeyStore> trustStore = nullptr;
    trustManagerFactory->init(trustStore);

    const auto trustManagers = trustManagerFactory->getTrustManagers();
    ASSERT_NE(trustManagers, nullptr);
    EXPECT_EQ(trustManagers->length, 1);
}

} // namespace
