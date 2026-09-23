#include <gtest/gtest.h>

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "net/jxx.net.InetAddress.h"

namespace {
TEST(InetAddressReachabilityValidationTest, NegativeTimeoutAndTtlAreRejected) {
    const auto address = ::jxx::net::InetAddress::getLoopbackAddress();
    ASSERT_NE(nullptr, address);
    EXPECT_THROW((void) address->isReachable(-1), ::jxx::lang::IllegalArgumentException);
    EXPECT_THROW((void) address->isReachable(nullptr, -1, 0), ::jxx::lang::IllegalArgumentException);
    EXPECT_THROW((void) address->isReachable(nullptr, 0, -1), ::jxx::lang::IllegalArgumentException);
}
}
