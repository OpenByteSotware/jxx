#include <gtest/gtest.h>

#include "net/jxx.net.NetworkInterface.h"

namespace {
TEST(NetworkInterfaceHardwareCopyTest, HardwareAddressIsDefensivelyCopied) {
    const auto interfaces =
        ::jxx::net::NetworkInterface::getNetworkInterfaces();

    ASSERT_NE(nullptr, interfaces);

    while (interfaces->hasMoreElements()) {
        const auto item = interfaces->nextElement();
        ASSERT_NE(nullptr, item);

        const auto first = item->getHardwareAddress();
        const auto second = item->getHardwareAddress();

        if (first != nullptr) {
            ASSERT_NE(nullptr, second);
            EXPECT_NE(first.get(), second.get());
            ASSERT_EQ(first->length, second->length);

            for (std::uint32_t index = 0;
                 index < first->length;
                 ++index) {
                EXPECT_EQ((*first)[index], (*second)[index]);
            }
        }
    }
}
}
