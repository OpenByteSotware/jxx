#include <gtest/gtest.h>
#include "ext/net/ssl/jxx.ext.net.ssl.SSLParameters.h"
#include "lang/jxx.lang.Object.h"

TEST(JxxSslParametersClientAuthTest, NeedClientAuthClearsWantClientAuth) {
    const auto parameters = ::jxx::NEW<::jxx::ext::net::ssl::SSLParameters>();
    parameters->setWantClientAuth(true);
    parameters->setNeedClientAuth(true);
    EXPECT_TRUE(parameters->getNeedClientAuth());
    EXPECT_FALSE(parameters->getWantClientAuth());
}

TEST(JxxSslParametersClientAuthTest, WantClientAuthClearsNeedClientAuth) {
    const auto parameters = ::jxx::NEW<::jxx::ext::net::ssl::SSLParameters>();
    parameters->setNeedClientAuth(true);
    parameters->setWantClientAuth(true);
    EXPECT_TRUE(parameters->getWantClientAuth());
    EXPECT_FALSE(parameters->getNeedClientAuth());
}
