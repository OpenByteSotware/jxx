#include <gtest/gtest.h>

#include "net/jxx.net.NetworkInterface.h"

namespace {
TEST(NetworkInterfaceSubinterfacesTest, SubinterfaceEnumerationIsNonNullAndContainsValidInterfaces) {
    const auto interfaces = ::jxx::net::NetworkInterface::getNetworkInterfaces();
    ASSERT_NE(nullptr, interfaces);
    while (interfaces->hasMoreElements()) {
        const auto item = interfaces->nextElement();
        ASSERT_NE(nullptr, item);
        const auto children = item->getSubInterfaces();
        ASSERT_NE(nullptr, children);
        while (children->hasMoreElements()) {
            const auto child = children->nextElement();
            ASSERT_NE(nullptr, child);
            EXPECT_TRUE(child->isVirtual());
            EXPECT_NE(nullptr, child->getParent());
        }
    }
}
}
