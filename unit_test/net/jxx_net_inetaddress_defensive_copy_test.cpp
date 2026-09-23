#include <gtest/gtest.h>

#include "lang/jxx.lang.buildin_array.h"
#include "net/jxx.net.InetAddress.h"

namespace {
TEST(InetAddressDefensiveCopyTest, ConstructorAndAccessorDoNotExposeAddressStorage) {
    auto bytes = ::jxx::NEW<::jxx::lang::ByteArrayType>(4);
    (*bytes)[0] = 127;
    (*bytes)[1] = 0;
    (*bytes)[2] = 0;
    (*bytes)[3] = 1;

    const auto address = ::jxx::net::InetAddress::getByAddress(bytes);
    ASSERT_NE(nullptr, address);

    (*bytes)[0] = 10;
    const auto first = address->getAddress();
    const auto second = address->getAddress();

    ASSERT_NE(nullptr, first);
    ASSERT_NE(nullptr, second);
    EXPECT_EQ(127, static_cast<unsigned char>((*first)[0]));
    EXPECT_NE(first.get(), second.get());
}
}
