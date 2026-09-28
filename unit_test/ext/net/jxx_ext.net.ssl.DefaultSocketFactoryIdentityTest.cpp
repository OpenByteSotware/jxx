#include <gtest/gtest.h>
#include "ext/net/ssl/jxx.ext.net.ssl.SSLSocketFactory.h"
namespace { TEST(DefaultSocketFactoryIdentityTest, DefaultFactoryHasStableIdentity) { const auto a=::jxx::ext::net::ssl::SSLSocketFactory::getDefault(); const auto b=::jxx::ext::net::ssl::SSLSocketFactory::getDefault(); ASSERT_NE(a,nullptr); EXPECT_EQ(a,b); } }
