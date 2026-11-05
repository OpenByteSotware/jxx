#include <gtest/gtest.h>
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Finally.h"
#include "lang/jxx.lang.GenericConversion.h"
#include "lang/jxx.lang.Integer.h"
#include "lang/jxx.lang.JxxArray.h"
#include "lang/jxx.lang.NumberFormatException.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.System.h"
#include "util/jxx.util.Objects.h"

namespace jxx::jdk8::lang {
    TEST(IntegerTest, ConstantsAreDocumented) {
        EXPECT_EQ(-2147483648, ::jxx::lang::Integer::MIN_VALUE);
        EXPECT_EQ(2147483647, ::jxx::lang::Integer::MAX_VALUE);
        EXPECT_EQ(32, ::jxx::lang::Integer::SIZE);
        EXPECT_EQ(4, ::jxx::lang::Integer::BYTES);
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::lang::Integer::TYPE, ::jxx::lang::Integer::TYPE));
    }

    TEST(IntegerTest, ConstructorsAndNumberConversionsWork) {
        ::jxx::Ptr<::jxx::lang::Integer> value = ::jxx::NEW<::jxx::lang::Integer>(258);
        EXPECT_EQ(static_cast<::jxx::lang::jbyte>(2), value->byteValue());
        EXPECT_EQ(static_cast<::jxx::lang::jshort>(258), value->shortValue());
        EXPECT_EQ(258, value->intValue());
        EXPECT_EQ(static_cast<::jxx::lang::jlong>(258), value->longValue());
        EXPECT_NEAR(static_cast<::jxx::lang::jfloat>(258.0), value->floatValue(), static_cast<::jxx::lang::jfloat>(0.0));
        EXPECT_NEAR(static_cast<::jxx::lang::jdouble>(258.0), value->doubleValue(), static_cast<::jxx::lang::jdouble>(0.0));
        EXPECT_EQ(42, (::jxx::NEW<::jxx::lang::Integer>(::jxx::NEW<::jxx::lang::String>("42")))->intValue());
    }

    TEST(IntegerTest, ParsingValueOfAndDecodeWork) {
        EXPECT_EQ(123, ::jxx::lang::Integer::parseInt(::jxx::NEW<::jxx::lang::String>("123")));
        EXPECT_EQ(255, ::jxx::lang::Integer::parseInt(::jxx::NEW<::jxx::lang::String>("ff"), 16));
        EXPECT_EQ(-10, ::jxx::lang::Integer::parseInt(::jxx::NEW<::jxx::lang::String>("-1010"), 2));
        EXPECT_EQ(123, ::jxx::lang::Integer::valueOf(::jxx::NEW<::jxx::lang::String>("123"))->intValue());
        EXPECT_EQ(255, ::jxx::lang::Integer::valueOf(::jxx::NEW<::jxx::lang::String>("ff"), 16)->intValue());
        EXPECT_EQ(16, ::jxx::lang::Integer::decode(::jxx::NEW<::jxx::lang::String>("0x10"))->intValue());
        EXPECT_EQ(8, ::jxx::lang::Integer::decode(::jxx::NEW<::jxx::lang::String>("010"))->intValue());
        EXPECT_EQ(-16, ::jxx::lang::Integer::decode(::jxx::NEW<::jxx::lang::String>("-#10"))->intValue());
    }

    TEST(IntegerTest, ParseIntRejectsOverflow) {
        ::jxx::lang::Integer::parseInt(::jxx::NEW<::jxx::lang::String>("2147483648"));
    }

    TEST(IntegerTest, UnsignedParsingAndFormattingWork) {
        EXPECT_EQ(-1, ::jxx::lang::Integer::parseUnsignedInt(::jxx::NEW<::jxx::lang::String>("4294967295")));
        EXPECT_EQ(-1, ::jxx::lang::Integer::parseUnsignedInt(::jxx::NEW<::jxx::lang::String>("ffffffff"), 16));
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::NEW<::jxx::lang::String>("4294967295"), ::jxx::lang::Integer::toUnsignedString(-1)));
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::NEW<::jxx::lang::String>("ffffffff"), ::jxx::lang::Integer::toUnsignedString(-1, 16)));
    }

    TEST(IntegerTest, FormattingSupportsCommonRadices) {
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::NEW<::jxx::lang::String>("123"), ::jxx::lang::String::valueOf(::jxx::lang::Integer::toString(123))));
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::NEW<::jxx::lang::String>("ff"), ::jxx::lang::String::valueOf(::jxx::lang::Integer::toString(255, 16))));
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::NEW<::jxx::lang::String>("ff"), ::jxx::lang::Integer::toHexString(255)));
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::NEW<::jxx::lang::String>("377"), ::jxx::lang::Integer::toOctalString(255)));
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::NEW<::jxx::lang::String>("11111111"), ::jxx::lang::Integer::toBinaryString(255)));
    }

    TEST(IntegerTest, EqualityHashAndComparisonWork) {
        ::jxx::Ptr<::jxx::lang::Integer> value = ::jxx::lang::Integer::valueOf(12);
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::NEW<::jxx::lang::String>("12"), ::jxx::lang::String::valueOf(value->toString())));
        EXPECT_TRUE(::jxx::util::Objects::equals(value, ::jxx::lang::Integer::valueOf(12)));
        EXPECT_FALSE(::jxx::util::Objects::equals(value, ::jxx::lang::Integer::valueOf(13)));
        EXPECT_EQ(12, value->hashCode());
        EXPECT_EQ(12, ::jxx::lang::Integer::hashCode(12));
        EXPECT_TRUE((value->compareTo(::jxx::lang::Integer::valueOf(13)) < 0));
        EXPECT_EQ(0, ::jxx::lang::Integer::compare(12, 12));
        EXPECT_TRUE((::jxx::lang::Integer::compareUnsigned(-1, 0) > 0));
    }

    TEST(IntegerTest, SystemPropertyAccessUsesDefaultAndRadixPrefixes) {
        ::jxx::Ptr<::jxx::lang::String> key = ::jxx::CONVERT<::jxx::Ptr<::jxx::lang::String>>("jdk8.lang.IntegerTest.number");
        ::jxx::Ptr<::jxx::lang::String> previous = ::jxx::lang::System::getProperty(key);
        {
            auto jxxFinally0 = ::jxx::lang::makeFinally([&]() {
                if ((previous == nullptr)) {
                    ::jxx::lang::System::clearProperty(key);
                } else {
                    ::jxx::lang::System::setProperty(key, previous);
                }
            });
            try {
                ::jxx::lang::System::setProperty(key, ::jxx::NEW<::jxx::lang::String>("0x20"));
                EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::lang::Integer::valueOf(32), ::jxx::lang::Integer::getInteger(key)));
                ::jxx::lang::System::clearProperty(key);
                EXPECT_EQ(::jxx::lang::Integer::getInteger(key), nullptr);
                EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::lang::Integer::valueOf(7), ::jxx::lang::Integer::getInteger(key, 7)));
                EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::lang::Integer::valueOf(9), ::jxx::lang::Integer::getInteger(key, ::jxx::lang::Integer::valueOf(9))));
            } catch (...) {
                jxxFinally0.runAndDismiss();
                throw;
            }
        }
    }

    TEST(IntegerTest, BitOperationsMatchThirtyTwoBitRepresentation) {
        EXPECT_EQ(32, ::jxx::lang::Integer::numberOfLeadingZeros(0));
        EXPECT_EQ(31, ::jxx::lang::Integer::numberOfLeadingZeros(1));
        EXPECT_EQ(32, ::jxx::lang::Integer::numberOfTrailingZeros(0));
        EXPECT_EQ(3, ::jxx::lang::Integer::numberOfTrailingZeros(8));
        EXPECT_EQ(8, ::jxx::lang::Integer::highestOneBit(10));
        EXPECT_EQ(2, ::jxx::lang::Integer::lowestOneBit(10));
        EXPECT_EQ(8, ::jxx::lang::Integer::bitCount(0xF0F0));
        EXPECT_EQ(0x23456781, ::jxx::lang::Integer::rotateLeft(0x12345678, 4));
        EXPECT_EQ(0x81234567, ::jxx::lang::Integer::rotateRight(0x12345678, 4));
        EXPECT_EQ(0x78563412, ::jxx::lang::Integer::reverseBytes(0x12345678));
        EXPECT_EQ(0x1E6A2C48, ::jxx::lang::Integer::reverse(0x12345678));
        EXPECT_EQ(-1, ::jxx::lang::Integer::signum(-100));
        EXPECT_EQ(0, ::jxx::lang::Integer::signum(0));
        EXPECT_EQ(1, ::jxx::lang::Integer::signum(100));
    }

    TEST(IntegerTest, UnsignedArithmeticTreatsBitsAsUnsigned) {
        EXPECT_EQ(2147483647, ::jxx::lang::Integer::divideUnsigned(-1, 2));
        EXPECT_EQ(1, ::jxx::lang::Integer::remainderUnsigned(-1, 2));
        EXPECT_EQ(static_cast<::jxx::lang::jlong>(4294967295), ::jxx::lang::Integer::toUnsignedLong(-1));
    }

    TEST(IntegerTest, ArithmeticHelpersWork) {
        EXPECT_EQ(20, ::jxx::lang::Integer::sum(8, 12));
        EXPECT_EQ(12, ::jxx::lang::Integer::max(8, 12));
        EXPECT_EQ(8, ::jxx::lang::Integer::min(8, 12));
    }

} // namespace jxx::jdk8::lang
