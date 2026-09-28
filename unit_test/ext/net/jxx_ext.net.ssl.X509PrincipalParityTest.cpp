#include <gtest/gtest.h>
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.X509Principal.h"
#include "lang/jxx.lang.String.h"
namespace {
TEST(X509PrincipalParityTest, PreservesDistinguishedName) {
    const auto principal = ::jxx::NEW<
        ::jxx::ext::net::ssl::internal::X509Principal>(
            ::jxx::NEW<::jxx::lang::String>("CN=example.test,O=JXX"));
    ASSERT_NE(principal->getName(), nullptr);
    EXPECT_EQ(principal->getName()->utf8(), "CN=example.test,O=JXX");
}
}
