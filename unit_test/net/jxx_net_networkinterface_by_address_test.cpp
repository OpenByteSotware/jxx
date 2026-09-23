#include <gtest/gtest.h>

#include "net/jxx.net.NetworkInterface.h"

namespace {
TEST(NetworkInterfaceByAddressTest, EnumeratedAddressesRoundTripToOwningInterface) {
    const auto interfaces =
        ::jxx::net::NetworkInterface::getNetworkInterfaces();

    ASSERT_NE(nullptr, interfaces);

    while (interfaces->hasMoreElements()) {
        const auto item = interfaces->nextElement();
        ASSERT_NE(nullptr, item);

        const auto addresses = item->getInetAddresses();
        ASSERT_NE(nullptr, addresses);

        while (addresses->hasMoreElements()) {
            const auto address = addresses->nextElement();
            ASSERT_NE(nullptr, address);

            const auto owner =
                ::jxx::net::NetworkInterface::getByInetAddress(address);

            ASSERT_NE(nullptr, owner);
            EXPECT_TRUE(item->equals(owner));
        }
    }
}
}
