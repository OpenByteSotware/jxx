#include <gtest/gtest.h>
#include <type_traits>
#include "security/jxx.security.KeyStore.h"
namespace {
TEST(KeyStoreSecretKeyEntryParityTest, ImplementsEntry) {
 EXPECT_TRUE((std::is_base_of_v<::jxx::security::KeyStore::Entry,::jxx::security::KeyStore::SecretKeyEntry>));
}
}
