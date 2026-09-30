#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "net/jxx.net.InetAddress.h"

namespace {

TEST(InetAddressPortAbsEthernetParity, NumericAddressToStringStartsWithSlash) {
    const auto address = ::jxx::net::InetAddress::getByName(
        ::jxx::NEW<::jxx::lang::String>("127.0.0.1"));

    const auto text = address->toString();
    ASSERT_NE(nullptr, text);
    EXPECT_EQ("/127.0.0.1", text->utf8());
    EXPECT_EQ("127.0.0.1", text->substring(1)->utf8());
}

TEST(InetAddressPortAbsEthernetParity, HostAddressWithoutHostStartsWithSlash) {
    const auto bytes = ::jxx::NEW<::jxx::lang::ByteArrayType>(4);
    (*bytes)[0] = 127;
    (*bytes)[1] = 0;
    (*bytes)[2] = 0;
    (*bytes)[3] = 1;

    const auto address = ::jxx::net::InetAddress::getByAddress(bytes);
    EXPECT_EQ("/127.0.0.1", address->toString()->utf8());
}

TEST(InetAddressPortAbsEthernetParity, ExplicitHostUsesHostSlashAddressForm) {
    const auto bytes = ::jxx::NEW<::jxx::lang::ByteArrayType>(4);
    (*bytes)[0] = 127;
    (*bytes)[1] = 0;
    (*bytes)[2] = 0;
    (*bytes)[3] = 1;

    const auto address = ::jxx::net::InetAddress::getByAddress(
        ::jxx::NEW<::jxx::lang::String>("device-name"),
        bytes);
    EXPECT_EQ("device-name/127.0.0.1", address->toString()->utf8());
}

} // namespace
