#include <gtest/gtest.h>
#include "ext/net/ssl/jxx.ext.net.ssl.KeyManagerFactory.h"
#include "ext/net/ssl/jxx.ext.net.ssl.TrustManagerFactory.h"
TEST(ManagerFactoryParityTest, DefaultAlgorithms){EXPECT_EQ(::jxx::ext::net::ssl::KeyManagerFactory::getDefaultAlgorithm()->utf8(),"SunX509");EXPECT_EQ(::jxx::ext::net::ssl::TrustManagerFactory::getDefaultAlgorithm()->utf8(),"PKIX");}
TEST(ManagerFactoryParityTest, RequiresInitialization){const auto f=::jxx::ext::net::ssl::TrustManagerFactory::getInstance(::jxx::NEW<::jxx::lang::String>("PKIX"));EXPECT_THROW(f->getTrustManagers(),::jxx::lang::IllegalStateException);f->init(::jxx::Ptr<::jxx::security::KeyStore>());EXPECT_EQ(f->getTrustManagers()->length,0);}
