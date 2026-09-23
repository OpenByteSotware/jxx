#include <gtest/gtest.h>

#include "net/jxx.net.InterfaceAddress.h"
#include "net/jxx.net.NetworkInterface.h"

namespace {
TEST(InterfaceAddressEqualityTest, ValuesAreReflexiveAndHashStable) {
    const auto interfaces = ::jxx::net::NetworkInterface::getNetworkInterfaces();
    ASSERT_NE(nullptr, interfaces);
    while (interfaces->hasMoreElements()) {
        const auto list = interfaces->nextElement()->getInterfaceAddresses();
        ASSERT_NE(nullptr, list);
        for (::jxx::lang::jint index = 0; index < list->size(); ++index) {
            const auto value = list->get(index);
            ASSERT_NE(nullptr, value);
            EXPECT_TRUE(value->equals(value));
            EXPECT_EQ(value->hashCode(), value->hashCode());
        }
    }
}
}
