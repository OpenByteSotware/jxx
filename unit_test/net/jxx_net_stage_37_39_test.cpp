#include <gtest/gtest.h>

#include "io/jxx.io.UnsupportedEncodingException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"
#include "net/jxx.net.InetAddress.h"
#include "net/jxx.net.InetSocketAddress.h"
#include "net/jxx.net.URLDecoder.h"
#include "net/jxx.net.URLEncoder.h"

TEST(InetSocketAddressParityTest, PortOnlyAndNullAddressUseWildcardAddress) {
    const auto byPort = ::jxx::NEW<::jxx::net::InetSocketAddress>(8080);
    const auto byNull = ::jxx::NEW<::jxx::net::InetSocketAddress>(
        ::jxx::Ptr<::jxx::net::InetAddress>(), 8080);
    const auto byPortAddress = byPort->getAddress();
    const auto byNullAddress = byNull->getAddress();
    ASSERT_NE(nullptr, byPortAddress);
    ASSERT_NE(nullptr, byNullAddress);
    EXPECT_TRUE(byPortAddress->isAnyLocalAddress());
    EXPECT_TRUE(byNullAddress->isAnyLocalAddress());
}

TEST(InetSocketAddressParityTest, UnresolvedHostEqualityIgnoresCase) {
    const auto upper = ::jxx::net::InetSocketAddress::createUnresolved(
        ::jxx::NEW<::jxx::lang::String>("EXAMPLE.TEST"), 80);
    const auto lower = ::jxx::net::InetSocketAddress::createUnresolved(
        ::jxx::NEW<::jxx::lang::String>("example.test"), 80);
    EXPECT_TRUE(upper->equals(lower));
    EXPECT_EQ(upper->hashCode(), lower->hashCode());
}

TEST(InetSocketAddressParityTest, NullHostIsRejected) {
    EXPECT_THROW(
        ::jxx::net::InetSocketAddress::createUnresolved(nullptr, 80),
        ::jxx::lang::NullPointerException);
}

TEST(FormUrlCodecParityTest, NullAndUnsupportedEncodingsUseJxxExceptions) {
    const auto text = ::jxx::NEW<::jxx::lang::String>("a b");
    EXPECT_THROW(::jxx::net::URLEncoder::encode(nullptr), ::jxx::lang::NullPointerException);
    EXPECT_THROW(::jxx::net::URLDecoder::decode(nullptr), ::jxx::lang::NullPointerException);
    EXPECT_THROW(
        ::jxx::net::URLEncoder::encode(text, ::jxx::NEW<::jxx::lang::String>("x-unsupported")),
        ::jxx::io::UnsupportedEncodingException);
    EXPECT_EQ("a+b", ::jxx::net::URLEncoder::encode(text)->utf8());
    EXPECT_EQ("a b", ::jxx::net::URLDecoder::decode(
        ::jxx::NEW<::jxx::lang::String>("a+b"))->utf8());
}
