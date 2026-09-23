#include <gtest/gtest.h>

#include "net/jxx.net.NetworkInterface.h"
#include "net/jxx.net.SocketException.h"

namespace {
TEST(NetworkInterfaceMtuTest, EnumeratedInterfacesReturnPositiveMtuOrSocketException) {
    const auto interfaces =
        ::jxx::net::NetworkInterface::getNetworkInterfaces();

    ASSERT_NE(nullptr, interfaces);

    while (interfaces->hasMoreElements()) {
        const auto item = interfaces->nextElement();
        ASSERT_NE(nullptr, item);

        try {
            EXPECT_GT(item->getMTU(), 0);
        } catch (const ::jxx::net::SocketException&) {
            SUCCEED();
        }
    }
}
}
