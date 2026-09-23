#include <gtest/gtest.h>

#include "net/jxx.net.NetworkInterface.h"

namespace {
TEST(NetworkInterfaceIdentityTest, NameIndexEqualityAndHashRoundTrip) {
    const auto interfaces = ::jxx::net::NetworkInterface::getNetworkInterfaces();
    ASSERT_NE(nullptr, interfaces);
    while (interfaces->hasMoreElements()) {
        const auto item = interfaces->nextElement();
        ASSERT_NE(nullptr, item);
        if (item->getName() != nullptr) {
            const auto byName = ::jxx::net::NetworkInterface::getByName(item->getName());
            ASSERT_NE(nullptr, byName);
            EXPECT_TRUE(item->equals(byName));
            EXPECT_EQ(item->hashCode(), byName->hashCode());
        }
        if (item->getIndex() > 0) {
            const auto byIndex = ::jxx::net::NetworkInterface::getByIndex(item->getIndex());
            ASSERT_NE(nullptr, byIndex);
            EXPECT_TRUE(item->equals(byIndex));
        }
    }
}
}
