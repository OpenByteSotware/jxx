#include <gtest/gtest.h>
#include <type_traits>
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslPropertyTrustManager.h"
namespace {
TEST(DefaultTrustStorePropertyParityTest, LoaderSurfaceIsAvailable) {
    EXPECT_TRUE((std::is_same_v<
        decltype(::jxx::ext::net::ssl::internal::loadDefaultPropertyTrustManagers()),
        ::jxx::Ptr<::jxx::ext::net::ssl::SSLContext::TrustManagerArray>>));
}
}
