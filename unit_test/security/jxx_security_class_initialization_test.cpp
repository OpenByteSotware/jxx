#include <gtest/gtest.h>

#include "jxx.initialize.h"
#include "security/jxx.security.ClassInitialization.h"
#include "security/jxx.security.AlgorithmConstraints.h"
#include "security/jxx.security.AlgorithmParameters.h"
#include "security/jxx.security.BasicPermission.h"
#include "security/jxx.security.CryptoPrimitive.h"
#include "security/jxx.security.GeneralSecurityException.h"
#include "security/jxx.security.Guard.h"
#include "security/jxx.security.Key.h"
#include "security/jxx.security.KeyManagementException.h"
#include "security/jxx.security.KeyStore.h"
#include "security/jxx.security.KeyStoreException.h"
#include "security/jxx.security.NoSuchAlgorithmException.h"
#include "security/jxx.security.NoSuchProviderException.h"
#include "security/jxx.security.Permission.h"
#include "security/jxx.security.Principal.h"
#include "security/jxx.security.PrivateKey.h"
#include "security/jxx.security.Provider.h"
#include "security/jxx.security.PublicKey.h"
#include "security/jxx.security.SecureString.h"
#include "security/jxx.security.UnrecoverableKeyException.h"

TEST(JxxSecurityClassInitialization, PublicTypesHaveClassMetadata)
{
    ::jxx::initialize();

    EXPECT_NE(::jxx::security::AlgorithmConstraints::Class(), nullptr);
    EXPECT_NE(::jxx::security::AlgorithmParameters::Class(), nullptr);
    EXPECT_NE(::jxx::security::BasicPermission::Class(), nullptr);
    EXPECT_NE(::jxx::security::CryptoPrimitive::Class(), nullptr);
    EXPECT_NE(::jxx::security::GeneralSecurityException::Class(), nullptr);
    EXPECT_NE(::jxx::security::Guard::Class(), nullptr);
    EXPECT_NE(::jxx::security::Key::Class(), nullptr);
    EXPECT_NE(::jxx::security::KeyManagementException::Class(), nullptr);
    EXPECT_NE(::jxx::security::KeyStore::Class(), nullptr);
    EXPECT_NE(::jxx::security::KeyStoreException::Class(), nullptr);
    EXPECT_NE(::jxx::security::NoSuchAlgorithmException::Class(), nullptr);
    EXPECT_NE(::jxx::security::NoSuchProviderException::Class(), nullptr);
    EXPECT_NE(::jxx::security::Permission::Class(), nullptr);
    EXPECT_NE(::jxx::security::Principal::Class(), nullptr);
    EXPECT_NE(::jxx::security::PrivateKey::Class(), nullptr);
    EXPECT_NE(::jxx::security::Provider::Class(), nullptr);
    EXPECT_NE(::jxx::security::PublicKey::Class(), nullptr);
    EXPECT_NE(::jxx::security::SecureString::Class(), nullptr);
    EXPECT_NE(::jxx::security::UnrecoverableKeyException::Class(), nullptr);
}

TEST(JxxSecurityClassInitialization, RepeatedInitializationIsIdempotent)
{
    EXPECT_NO_THROW(::jxx::security::initializeClasses());
    EXPECT_NO_THROW(::jxx::security::initializeClasses());
    EXPECT_EQ(::jxx::security::KeyStore::Class(), ::jxx::security::KeyStore::Class());
}
