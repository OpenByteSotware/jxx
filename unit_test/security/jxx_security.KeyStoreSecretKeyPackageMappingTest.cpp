#include <gtest/gtest.h>
#include <type_traits>

#include "ext/crypto/jxx.ext.crypto.SecretKey.h"
#include "ext/security/auth/jxx.ext.security.auth.Destroyable.h"
#include "security/jxx.security.KeyStore.h"

namespace {

TEST(KeyStoreSecretKeyPackageMappingTest, UsesExtCryptoSecretKey) {
    EXPECT_TRUE((std::is_base_of_v<
        ::jxx::security::Key,
        ::jxx::ext::crypto::SecretKey>));
    EXPECT_TRUE((std::is_base_of_v<
        ::jxx::ext::security::auth::Destroyable,
        ::jxx::ext::crypto::SecretKey>));
}

} // namespace
