#include <gtest/gtest.h>

#include "net/jxx.net.NetworkInterface.h"

namespace {
TEST(NetworkInterfaceCapabilitiesTest, EnumeratedInterfacesExposeCoherentCapabilities) {
    const auto interfaces = ::jxx::net::NetworkInterface::getNetworkInterfaces();
    ASSERT_NE(nullptr, interfaces);
    while (interfaces->hasMoreElements()) {
        const auto item = interfaces->nextElement();
        ASSERT_NE(nullptr, item);
        EXPECT_GT(item->getIndex(), 0);
        EXPECT_GE(item->getMTU(), 0);
        if (item->isLoopback()) {
            EXPECT_FALSE(item->isPointToPoint());
        }
    }
}
}
