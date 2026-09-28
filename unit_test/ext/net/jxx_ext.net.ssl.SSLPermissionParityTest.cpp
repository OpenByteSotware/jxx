#include <gtest/gtest.h>

#include <type_traits>

#include "ext/net/ssl/jxx.ext.net.ssl.SSLPermission.h"
#include "lang/jxx.lang.String.h"
#include "security/jxx.security.BasicPermission.h"

namespace {

TEST(SSLPermissionParityTest, ExtendsBasicPermission) {
    EXPECT_TRUE((std::is_base_of_v<
        ::jxx::security::BasicPermission,
        ::jxx::ext::net::ssl::SSLPermission>));
}

TEST(SSLPermissionParityTest, PreservesPermissionTarget) {
    const auto permission =
        ::jxx::NEW<::jxx::ext::net::ssl::SSLPermission>(
            ::jxx::NEW<::jxx::lang::String>(
                "setDefaultSSLContext"));

    ASSERT_NE(permission->getName(), nullptr);
    EXPECT_EQ(
        permission->getName()->utf8(),
        "setDefaultSSLContext");
    EXPECT_TRUE(permission->getActions()->utf8().empty());
}

} // namespace
