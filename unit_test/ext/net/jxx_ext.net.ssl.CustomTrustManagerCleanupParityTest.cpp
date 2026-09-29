#include <gtest/gtest.h>
#include <type_traits>
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslManagerBridge.h"
namespace {
TEST(CustomTrustManagerCleanupParityTest, VerifyPeerSurfaceRemainsAvailable) {
 EXPECT_TRUE((std::is_member_function_pointer_v<decltype(&::jxx::ext::net::ssl::internal::OpenSslManagerBridge::verifyPeer)>));
}
}
