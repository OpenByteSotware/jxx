#include <gtest/gtest.h>

#include <string>

#include "lang/jxx.lang.String.h"
#include "net/jxx.net.NetworkInterface.h"

namespace {
TEST(NetworkInterfaceToStringTest, StringContainsInterfaceNameWhenAvailable) {
    const auto interfaces =
        ::jxx::net::NetworkInterface::getNetworkInterfaces();

    ASSERT_NE(nullptr, interfaces);

    while (interfaces->hasMoreElements()) {
        const auto item = interfaces->nextElement();
        ASSERT_NE(nullptr, item);

        const auto value = item->toString();
        ASSERT_NE(nullptr, value);

        const auto name = item->getName();
        if (name != nullptr) {
            EXPECT_NE(
                std::string::npos,
                value->utf8().find(
                    name->utf8()));
        }
    }
}
}
