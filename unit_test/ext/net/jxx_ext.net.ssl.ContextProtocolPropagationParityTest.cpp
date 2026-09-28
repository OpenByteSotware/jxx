#include <gtest/gtest.h>
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslProtocolPolicy.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
namespace {
TEST(ContextProtocolPropagationParityTest, ExactContextNamesAndEnabledRangesAgree) {
    using namespace ::jxx::ext::net::ssl::internal;
    const auto names = supportedProtocolNames();
    ASSERT_EQ(names.size(), 4U);
    EXPECT_EQ(enabledProtocolRange({"TLSv1"}), protocolRange(::jxx::NEW<::jxx::lang::String>("TLSv1")));
    EXPECT_EQ(enabledProtocolRange({"TLSv1.1"}), protocolRange(::jxx::NEW<::jxx::lang::String>("TLSv1.1")));
    EXPECT_EQ(enabledProtocolRange({"TLSv1.2", "TLSv1.3"}), protocolRange(::jxx::NEW<::jxx::lang::String>("TLS")));
}
TEST(ContextProtocolPropagationParityTest, EmptyEnabledSelectionIsRejected) {
    EXPECT_THROW((::jxx::ext::net::ssl::internal::enabledProtocolRange({})), ::jxx::lang::IllegalArgumentException);
}
}
