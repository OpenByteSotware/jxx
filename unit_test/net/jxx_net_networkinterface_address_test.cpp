#include <gtest/gtest.h>
#include "net/jxx.net.NetworkInterface.h"
namespace {
TEST(NetworkInterfaceAddressTest, EnumeratedInterfacesExposeNonNullAddressesWhenPresent) {
    const auto interfaces=::jxx::net::NetworkInterface::getNetworkInterfaces();
    ASSERT_NE(nullptr,interfaces);
    while(interfaces->hasMoreElements()) {
        const auto interfaceValue=interfaces->nextElement();
        ASSERT_NE(nullptr,interfaceValue);
        const auto addresses=interfaceValue->getInetAddresses();
        ASSERT_NE(nullptr,addresses);
        while(addresses->hasMoreElements()) EXPECT_NE(nullptr,addresses->nextElement());
    }
}
}
