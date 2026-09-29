#include <gtest/gtest.h>
#include <type_traits>
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslKeyStoreKeyManager.h"
namespace {
TEST(KeyStoreKeyManagerIssuerParityTest, ManagerImplementsExtendedKeyManager) {
 EXPECT_TRUE((std::is_base_of_v<::jxx::ext::net::ssl::X509ExtendedKeyManager,::jxx::ext::net::ssl::internal::OpenSslKeyStoreKeyManager>));
}
}
