#include <gtest/gtest.h>
#include <type_traits>
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslPropertyKeyManager.h"
namespace {
TEST(DefaultPkcs12PropertyIdentityParityTest, LoaderSurfaceIsAvailable) {
    EXPECT_TRUE((std::is_same_v<
        decltype(::jxx::ext::net::ssl::internal::loadDefaultPropertyKeyManagers()),
        ::jxx::Ptr<::jxx::ext::net::ssl::SSLContext::KeyManagerArray>>));
}
}
