#include <gtest/gtest.h>

#include "net/jxx.net.InetAddress.h"
#include "net/jxx.net.InterfaceAddress.h"
#include "net/jxx.net.NetworkInterface.h"

namespace {
TEST(InterfaceAddressPrefixTest, PrefixLengthsFitAddressFamilies) {
    const auto interfaces = ::jxx::net::NetworkInterface::getNetworkInterfaces();
    ASSERT_NE(nullptr, interfaces);
    while (interfaces->hasMoreElements()) {
        const auto list = interfaces->nextElement()->getInterfaceAddresses();
        ASSERT_NE(nullptr, list);
        for (::jxx::lang::jint index = 0; index < list->size(); ++index) {
            const auto value = list->get(index);
            ASSERT_NE(nullptr, value);
            const auto address = value->getAddress();
            ASSERT_NE(nullptr, address);
            const auto bytes = address->getAddress();
            ASSERT_NE(nullptr, bytes);
            EXPECT_GE(value->getNetworkPrefixLength(), 0);
            EXPECT_LE(value->getNetworkPrefixLength(),
                bytes->length == 4U ? 32 : 128);
        }
    }
}
}
