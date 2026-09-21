#include <gtest/gtest.h>

#include <limits>

#include "lang/jxx.lang.ArithmeticException.h"
#include "lang/jxx.lang.Integer.h"
#include "lang/jxx.lang.Long.h"
#include "lang/jxx.lang.NumberFormatException.h"
#include "lang/jxx.lang.String.h"

namespace {
using ::jxx::lang::ArithmeticException;
using ::jxx::lang::Integer;
using ::jxx::lang::Long;
using ::jxx::lang::NumberFormatException;
using ::jxx::lang::String;

TEST(IntegerJava8ParityTest, FormatsSignedAndUnsignedValuesAcrossRadices) {
    EXPECT_EQ("-2147483648", Integer::toString(Integer::MIN_VALUE, 10)->utf8());
    EXPECT_EQ("ffffffff", Integer::toHexString(-1)->utf8());
    EXPECT_EQ("37777777777", Integer::toOctalString(-1)->utf8());
    EXPECT_EQ("4294967295", Integer::toUnsignedString(-1)->utf8());
    EXPECT_EQ("101010", Integer::toBinaryString(42)->utf8());
}

TEST(IntegerJava8ParityTest, ParsesAndComputesUnsignedValues) {
    EXPECT_EQ(-1, Integer::parseUnsignedInt(::jxx::NEW<String>("4294967295")));
    EXPECT_EQ(-1, Integer::parseUnsignedInt(::jxx::NEW<String>("ffffffff"), 16));
    EXPECT_GT(Integer::compareUnsigned(-1, 0), 0);
    EXPECT_EQ(2147483647, Integer::divideUnsigned(-2, 2));
    EXPECT_EQ(1, Integer::remainderUnsigned(-1, 2));
    EXPECT_THROW(Integer::parseUnsignedInt(::jxx::NEW<String>("4294967296")), NumberFormatException);
    EXPECT_THROW(Integer::divideUnsigned(1, 0), ArithmeticException);
}

TEST(IntegerJava8ParityTest, ImplementsBitTwiddlingOperations) {
    EXPECT_EQ(0x40000000, Integer::highestOneBit(0x60000000));
    EXPECT_EQ(8, Integer::lowestOneBit(40));
    EXPECT_EQ(31, Integer::numberOfLeadingZeros(1));
    EXPECT_EQ(3, Integer::numberOfTrailingZeros(40));
    EXPECT_EQ(32, Integer::bitCount(-1));
    EXPECT_EQ(2, Integer::rotateLeft(1, 1));
    EXPECT_EQ(Integer::MIN_VALUE, Integer::rotateRight(1, 1));
    EXPECT_EQ(0x78563412, Integer::reverseBytes(0x12345678));
}

TEST(LongJava8ParityTest, FormatsParsesAndComputesUnsignedValues) {
    EXPECT_EQ("18446744073709551615", Long::toUnsignedString(-1)->utf8());
    EXPECT_EQ("ffffffffffffffff", Long::toHexString(-1)->utf8());
    EXPECT_EQ(-1, Long::parseUnsignedLong(
        ::jxx::NEW<String>("18446744073709551615")));
    EXPECT_GT(Long::compareUnsigned(-1, 0), 0);
    EXPECT_EQ(std::numeric_limits<::jxx::lang::jlong>::max(),
              Long::divideUnsigned(-2, 2));
    EXPECT_EQ(1, Long::remainderUnsigned(-1, 2));
    EXPECT_THROW(Long::parseUnsignedLong(
        ::jxx::NEW<String>("18446744073709551616")), NumberFormatException);
}

TEST(LongJava8ParityTest, ImplementsBitTwiddlingOperations) {
    EXPECT_EQ(INT64_C(0x4000000000000000),
              Long::highestOneBit(INT64_C(0x6000000000000000)));
    EXPECT_EQ(8, Long::lowestOneBit(40));
    EXPECT_EQ(63, Long::numberOfLeadingZeros(1));
    EXPECT_EQ(3, Long::numberOfTrailingZeros(40));
    EXPECT_EQ(64, Long::bitCount(-1));
    EXPECT_EQ(INT64_C(0x0807060504030201),
              Long::reverseBytes(INT64_C(0x0102030405060708)));
}

TEST(IntegerLongJava8ParityTest, FunctionalHelpersPreservePrimitiveOverflowSemantics) {
    EXPECT_EQ(Integer::MIN_VALUE, Integer::sum(Integer::MAX_VALUE, 1));
    EXPECT_EQ(Long::MIN_VALUE, Long::sum(Long::MAX_VALUE, 1));
    EXPECT_EQ(-1, Integer::signum(-42));
    EXPECT_EQ(1, Long::signum(42));
    EXPECT_EQ(-2, Integer::min(-2, 3));
    EXPECT_EQ(3, Long::max(-2, 3));
}
} // namespace
