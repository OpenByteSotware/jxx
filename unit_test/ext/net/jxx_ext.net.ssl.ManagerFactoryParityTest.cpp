#include <gtest/gtest.h>
#include "ext/net/ssl/jxx.ext.net.ssl.KeyManagerFactory.h"
#include "ext/net/ssl/jxx.ext.net.ssl.TrustManagerFactory.h"
TEST(ManagerFactoryParityTest, DefaultAlgorithms)
{
	EXPECT_EQ(::jxx::ext::net::ssl::KeyManagerFactory::getDefaultAlgorithm()->utf8(), "SunX509"); 
	EXPECT_EQ(::jxx::ext::net::ssl::TrustManagerFactory::getDefaultAlgorithm()->utf8(), "PKIX");
}
TEST(ManagerFactoryParityTest, RequiresInitialization)
{
    const auto factory =
        ::jxx::ext::net::ssl::TrustManagerFactory::getInstance(
            ::jxx::NEW<::jxx::lang::String>("PKIX"));

    EXPECT_THROW(
        factory->getTrustManagers(),
        ::jxx::lang::IllegalStateException);

    factory->init(
        ::jxx::Ptr<::jxx::security::KeyStore>());

    const auto managers =
        factory->getTrustManagers();

    ASSERT_NE(nullptr, managers);
    EXPECT_EQ(1, managers->length);
    EXPECT_NE(nullptr, (*managers)[0]);
}