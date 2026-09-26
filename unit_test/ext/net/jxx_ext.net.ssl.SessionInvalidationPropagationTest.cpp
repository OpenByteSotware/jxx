#include <gtest/gtest.h>
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslSession.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslSessionContext.h"

TEST(SessionInvalidationPropagationTest, RemovesPublicSessionFromContext) {
    using namespace ::jxx::ext::net::ssl::internal;
    const auto context = ::jxx::NEW<OpenSslSessionContext>();
    const auto id = ::jxx::NEW<
        ::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>(2);
    (*id)[0] = 1;
    (*id)[1] = 2;
    const auto session = ::jxx::NEW<OpenSslSession>(
        ::jxx::NEW<::jxx::lang::String>("TLS_TEST"),
        ::jxx::NEW<::jxx::lang::String>("TLSv1.2"),
        ::jxx::NEW<::jxx::lang::String>("localhost"),
        443, nullptr, nullptr, id, context);
    context->registerSession(session);
    ASSERT_NE(context->getSession(id), nullptr);
    session->invalidate();
    EXPECT_EQ(context->getSession(id), nullptr);
    EXPECT_FALSE(session->isValid());
}
