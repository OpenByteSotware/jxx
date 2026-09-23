#include <gtest/gtest.h>

#include "net/jxx.net.NetworkInterface.h"

namespace {
TEST(NetworkInterfaceHardwareAddressTest, HardwareAddressesHavePositiveLengthWhenPresent) {
    const auto interfaces = ::jxx::net::NetworkInterface::getNetworkInterfaces();
    ASSERT_NE(nullptr, interfaces);
    while (interfaces->hasMoreElements()) {
        const auto item = interfaces->nextElement();
        ASSERT_NE(nullptr, item);
        const auto address = item->getHardwareAddress();
        if (address != nullptr) {
            EXPECT_GT(address->length, 0U);
        }
    }
}
}
