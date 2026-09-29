#include <gtest/gtest.h>
#include <type_traits>
#include "security/jxx.security.KeyStore.h"
namespace {
TEST(KeyStoreEntryParityTest, EntryTypesImplementEntryContract) {
 EXPECT_TRUE((std::is_base_of_v<::jxx::security::KeyStore::Entry,::jxx::security::KeyStore::PrivateKeyEntry>));
 EXPECT_TRUE((std::is_base_of_v<::jxx::security::KeyStore::Entry,::jxx::security::KeyStore::TrustedCertificateEntry>));
 EXPECT_TRUE((std::is_base_of_v<::jxx::security::KeyStore::ProtectionParameter,::jxx::security::KeyStore::PasswordProtection>));
}
}
