#include <gtest/gtest.h>
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslSessionContext.h"
#include "lang/jxx.lang.IllegalArgumentException.h"

TEST(SessionContextResumptionParityTest, RejectsNegativeLimits) {
    const auto context = ::jxx::NEW<
        ::jxx::ext::net::ssl::internal::OpenSslSessionContext>();
    EXPECT_THROW(context->setSessionTimeout(-1), ::jxx::lang::IllegalArgumentException);
    EXPECT_THROW(context->setSessionCacheSize(-1), ::jxx::lang::IllegalArgumentException);
}

TEST(SessionContextResumptionParityTest, EmptyIdentifierDoesNotCrash) {
    const auto context = ::jxx::NEW<
        ::jxx::ext::net::ssl::internal::OpenSslSessionContext>();
    const auto empty = ::jxx::NEW<
        ::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>(0);
    EXPECT_EQ(context->getSession(empty), nullptr);
}
