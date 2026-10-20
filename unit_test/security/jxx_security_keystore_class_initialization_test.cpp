#include <gtest/gtest.h>

#include "security/jxx.security.ClassInitialization.h"
#include "security/jxx.security.KeyStore.h"
#include "jxx.initialize.h"

namespace
{
    template <typename T>
    void expectClassMetadata()
    {
        const auto type = T::Class();
        ASSERT_NE(type, nullptr);
        ASSERT_NE(type->getName(), nullptr);
        EXPECT_FALSE(type->getName()->utf8().empty());
    }

    TEST(SecurityClassInitialization, RegistersPublicKeyStoreNestedTypes)
    {
        ::jxx::initialize();

        expectClassMetadata<::jxx::security::KeyStore>();
        expectClassMetadata<::jxx::security::KeyStore::Entry>();
        expectClassMetadata<::jxx::security::KeyStore::ProtectionParameter>();
        expectClassMetadata<::jxx::security::KeyStore::PasswordProtection>();
        expectClassMetadata<::jxx::security::KeyStore::PrivateKeyEntry>();
        expectClassMetadata<::jxx::security::KeyStore::SecretKeyEntry>();
        expectClassMetadata<::jxx::security::KeyStore::TrustedCertificateEntry>();
        expectClassMetadata<::jxx::security::KeyStore::Builder>();
    }
}
