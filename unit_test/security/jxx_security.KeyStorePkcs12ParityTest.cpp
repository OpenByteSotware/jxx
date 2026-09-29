#include <gtest/gtest.h>
#include "security/jxx.security.KeyStore.h"
#include "lang/jxx.lang.String.h"
namespace { TEST(KeyStorePkcs12ParityTest, Pkcs12InstanceSurface) { const auto s=::jxx::security::KeyStore::getInstance(::jxx::NEW<::jxx::lang::String>("PKCS12")); ASSERT_NE(s,nullptr); EXPECT_EQ(s->getType()->utf8(),"PKCS12"); } }
