#include <gtest/gtest.h>

#include "lang/jxx.lang.buildin_array.h"
#include "net/jxx.net.InetAddress.h"
#include "net/jxx.net.UnknownHostException.h"

namespace {
TEST(InetAddressInvalidLengthTest, UnsupportedAddressLengthsThrowUnknownHostException) {
    auto bytes = ::jxx::NEW<::jxx::lang::ByteArrayType>(5);
    EXPECT_THROW(
        (void) ::jxx::net::InetAddress::getByAddress(bytes),
        ::jxx::net::UnknownHostException);
}
}
