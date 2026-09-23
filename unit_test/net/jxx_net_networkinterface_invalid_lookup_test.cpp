#include <gtest/gtest.h>

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "net/jxx.net.NetworkInterface.h"

namespace {
TEST(NetworkInterfaceInvalidLookupTest, InvalidArgumentsUseJxxExceptions) {
    const ::jxx::Ptr<::jxx::lang::String> noName;
    EXPECT_THROW(
        (void) ::jxx::net::NetworkInterface::getByName(noName),
        ::jxx::lang::NullPointerException);

    EXPECT_THROW(
        (void) ::jxx::net::NetworkInterface::getByIndex(0),
        ::jxx::lang::IllegalArgumentException);
}
}
