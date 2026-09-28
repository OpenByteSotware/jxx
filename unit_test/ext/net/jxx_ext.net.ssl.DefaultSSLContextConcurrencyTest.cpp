#include <gtest/gtest.h>
#include "ext/net/ssl/jxx.ext.net.ssl.SSLContext.h"
namespace { TEST(DefaultSSLContextConcurrencyTest, DefaultContextHasStableIdentity) { const auto a=::jxx::ext::net::ssl::SSLContext::getDefault(); const auto b=::jxx::ext::net::ssl::SSLContext::getDefault(); ASSERT_NE(a,nullptr); EXPECT_EQ(a,b); } }
