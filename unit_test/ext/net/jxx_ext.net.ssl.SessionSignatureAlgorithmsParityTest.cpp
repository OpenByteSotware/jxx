#include <gtest/gtest.h>
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslSession.h"
namespace {
TEST(SessionSignatureAlgorithmsParityTest, ReturnsDefensiveArrayCopies) {
    using Session = ::jxx::ext::net::ssl::internal::OpenSslSession;
    const auto local = ::jxx::NEW<Session::StringArray>(1);
    (*local)[0] = ::jxx::NEW<::jxx::lang::String>("SHA256withRSA");
    const auto session = ::jxx::NEW<Session>(
        ::jxx::NEW<::jxx::lang::String>("TLS_AES_128_GCM_SHA256"),
        ::jxx::NEW<::jxx::lang::String>("TLSv1.3"), nullptr, -1,
        nullptr, nullptr, nullptr, nullptr, local, nullptr);
    const auto first = session->getLocalSupportedSignatureAlgorithms();
    const auto second = session->getLocalSupportedSignatureAlgorithms();
    ASSERT_NE(first, nullptr);
    ASSERT_NE(second, nullptr);
    EXPECT_NE(first, second);
    ASSERT_EQ(first->length, 1);
    EXPECT_EQ((*first)[0]->utf8(), "SHA256withRSA");
    EXPECT_EQ(session->getPeerSupportedSignatureAlgorithms()->length, 0);
}
}
