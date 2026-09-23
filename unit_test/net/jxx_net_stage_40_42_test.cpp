#include <gtest/gtest.h>

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"
#include "net/jxx.net.IDN.h"
#include "net/jxx.net.URI.h"
#include "net/jxx.net.URL.h"

TEST(UriNullArgumentParityTest, ResolveRelativizeAndCompareRejectNull) {
    const auto base = ::jxx::NEW<::jxx::net::URI>(
        ::jxx::NEW<::jxx::lang::String>("http://example.test/base/"));
    EXPECT_THROW(base->resolve(::jxx::Ptr<::jxx::net::URI>()), ::jxx::lang::NullPointerException);
    EXPECT_THROW(base->relativize(nullptr), ::jxx::lang::NullPointerException);
    EXPECT_THROW(base->compareTo(nullptr), ::jxx::lang::NullPointerException);
}

TEST(UrlValueParityTest, SchemeAndHostComparisonIgnoreAsciiCase) {
    const auto upper = ::jxx::NEW<::jxx::net::URL>(
        ::jxx::NEW<::jxx::lang::String>("HTTP://EXAMPLE.TEST/path?q=1#part"));
    const auto lower = ::jxx::NEW<::jxx::net::URL>(
        ::jxx::NEW<::jxx::lang::String>("http://example.test/path?q=1#part"));
    EXPECT_TRUE(upper->sameFile(lower));
    EXPECT_TRUE(upper->equals(lower));
    EXPECT_EQ(upper->hashCode(), lower->hashCode());
}

TEST(IdnArgumentParityTest, RejectsNullAndUnknownFlagBits) {
    EXPECT_THROW(::jxx::net::IDN::toASCII(nullptr), ::jxx::lang::NullPointerException);
    EXPECT_THROW(::jxx::net::IDN::toUnicode(nullptr), ::jxx::lang::NullPointerException);
    EXPECT_THROW(
        ::jxx::net::IDN::toASCII(::jxx::NEW<::jxx::lang::String>("example"), 0x40),
        ::jxx::lang::IllegalArgumentException);
}
