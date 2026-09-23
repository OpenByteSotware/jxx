#include <gtest/gtest.h>

#include "net/jxx.net.NetworkInterface.h"

namespace {
TEST(NetworkInterfaceParentVirtualTest, ParentAndVirtualStateAreCoherent) {
    const auto interfaces = ::jxx::net::NetworkInterface::getNetworkInterfaces();
    ASSERT_NE(nullptr, interfaces);
    while (interfaces->hasMoreElements()) {
        const auto item = interfaces->nextElement();
        ASSERT_NE(nullptr, item);
        const auto parent = item->getParent();
        if (parent != nullptr) {
            EXPECT_TRUE(item->isVirtual());
            EXPECT_NE(nullptr, parent->getName());
        }
    }
}
}
