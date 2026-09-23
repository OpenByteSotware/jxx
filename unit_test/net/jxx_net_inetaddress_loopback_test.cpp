#include <gtest/gtest.h>

#include "net/jxx.net.InetAddress.h"

namespace {
TEST(InetAddressLoopbackTest, LoopbackAddressIsNonNullAndClassifiedAsLoopback) {
    const auto address = ::jxx::net::InetAddress::getLoopbackAddress();
    ASSERT_NE(nullptr, address);
    EXPECT_TRUE(address->isLoopbackAddress());
    EXPECT_TRUE(address->isReachable(0));
}
}
