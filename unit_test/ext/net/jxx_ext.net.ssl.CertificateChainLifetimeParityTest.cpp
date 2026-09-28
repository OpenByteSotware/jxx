#include <gtest/gtest.h>
#include <type_traits>
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslManagerBridge.h"
namespace {
TEST(CertificateChainLifetimeParityTest, IdentitySelectionSurfaceRemainsStable) {
    using Bridge = ::jxx::ext::net::ssl::internal::OpenSslManagerBridge;
    EXPECT_TRUE((std::is_member_function_pointer_v<
        decltype(&Bridge::selectServerIdentity)>));
    EXPECT_TRUE((std::is_member_function_pointer_v<
        decltype(&Bridge::selectClientCertificate)>));
}
}
