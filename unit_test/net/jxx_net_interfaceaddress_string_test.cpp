#include <gtest/gtest.h>

#include <string>

#include "lang/jxx.lang.String.h"
#include "net/jxx.net.InterfaceAddress.h"
#include "net/jxx.net.NetworkInterface.h"

namespace {
TEST(InterfaceAddressStringTest, StringContainsSlashPrefixSeparator) {
    const auto interfaces = ::jxx::net::NetworkInterface::getNetworkInterfaces();
    ASSERT_NE(nullptr, interfaces);
    while (interfaces->hasMoreElements()) {
        const auto list = interfaces->nextElement()->getInterfaceAddresses();
        ASSERT_NE(nullptr, list);
        for (::jxx::lang::jint index = 0; index < list->size(); ++index) {
            const auto text = list->get(index)->toString();
            ASSERT_NE(nullptr, text);
            EXPECT_NE(std::string::npos, text->utf8().find('/'));
        }
    }
}
}
