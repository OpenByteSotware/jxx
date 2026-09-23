#include <gtest/gtest.h>

#include "net/jxx.net.InetAddress.h"

namespace {
TEST(InetAddressLocalHostTest, LocalHostResolvesToAnAddress) {
    const auto address = ::jxx::net::InetAddress::getLocalHost();
    ASSERT_NE(nullptr, address);
    ASSERT_NE(nullptr, address->getHostAddress());
    ASSERT_NE(nullptr, address->getAddress());
}
}
