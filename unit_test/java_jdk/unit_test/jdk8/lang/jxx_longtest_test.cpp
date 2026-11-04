#include <gtest/gtest.h>
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Finally.h"
#include "lang/jxx.lang.GenericConversion.h"
#include "lang/jxx.lang.JxxArray.h"
#include "lang/jxx.lang.Long.h"
#include "lang/jxx.lang.NumberFormatException.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.System.h"
#include "util/jxx.util.Objects.h"

namespace jxx::jdk8::lang {
    TEST(LongTest, ConstantsAreDocumented) {
        EXPECT_EQ(::jxx::lang::Long::MIN_VALUE, (-static_cast<::jxx::lang::jlong>(9223372036854775807) - static_cast<::jxx::lang::jlong>(1)));
        EXPECT_EQ(::jxx::lang::Long::MAX_VALUE, static_cast<::jxx::lang::jlong>(9223372036854775807));
        EXPECT_EQ(64, ::jxx::lang::Long::SIZE);
        EXPECT_EQ(8, ::jxx::lang::Long::BYTES);
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::lang::Long::TYPE, ::jxx::lang::Long::TYPE));
    }

    TEST(LongTest, ConstructorsAndNumberConversionsWork) {
        ::jxx::Ptr<::jxx::lang::Long> value = ::jxx::NEW<::jxx::lang::Long>(static_cast<::jxx::lang::jlong>(258));
        EXPECT_EQ(static_cast<::jxx::lang::jbyte>(2), value->byteValue());
        EXPECT_EQ(static_cast<::jxx::lang::jshort>(258), value->shortValue());
        EXPECT_EQ(258, value->intValue());
        EXPECT_EQ(static_cast<::jxx::lang::jlong>(258), value->longValue());
        EXPECT_NEAR(static_cast<::jxx::lang::jfloat>(258.0), value->floatValue(), static_cast<::jxx::lang::jfloat>(0.0));
        EXPECT_NEAR(static_cast<::jxx::lang::jdouble>(258.0), value->doubleValue(), static_cast<::jxx::lang::jdouble>(0.0));
        EXPECT_TRUE(::jxx::util::Objects::equals(static_cast<::jxx::lang::jlong>(42), (::jxx::NEW<::jxx::lang::Long>(::jxx::NEW<::jxx::lang::String>("42")))->longValue()));
    }

    TEST(LongTest, ParsingValueOfAndDecodeWork) {
        EXPECT_EQ(static_cast<::jxx::lang::jlong>(123), ::jxx::lang::Long::parseLong(::jxx::NEW<::jxx::lang::String>("123")));
        EXPECT_EQ(static_cast<::jxx::lang::jlong>(255), ::jxx::lang::Long::parseLong(::jxx::NEW<::jxx::lang::String>("ff"), 16));
        EXPECT_EQ(-static_cast<::jxx::lang::jlong>(10), ::jxx::lang::Long::parseLong(::jxx::NEW<::jxx::lang::String>("-1010"), 2));
        EXPECT_TRUE(::jxx::util::Objects::equals(static_cast<::jxx::lang::jlong>(123), ::jxx::lang::String::valueOf(::jxx::lang::Long::valueOf(::jxx::NEW<::jxx::lang::String>("123"))->longValue())));
        EXPECT_TRUE(::jxx::util::Objects::equals(static_cast<::jxx::lang::jlong>(255), ::jxx::lang::String::valueOf(::jxx::lang::Long::valueOf(::jxx::NEW<::jxx::lang::String>("ff"), 16)->longValue())));
        EXPECT_EQ(static_cast<::jxx::lang::jlong>(16), ::jxx::lang::Long::decode(::jxx::NEW<::jxx::lang::String>("0x10"))->longValue());
        EXPECT_EQ(static_cast<::jxx::lang::jlong>(8), ::jxx::lang::Long::decode(::jxx::NEW<::jxx::lang::String>("010"))->longValue());
    }

    TEST(LongTest, ParseLongRejectsOverflow) {
        ::jxx::lang::Long::parseLong(::jxx::NEW<::jxx::lang::String>("9223372036854775808"));
    }

    TEST(LongTest, UnsignedParsingAndFormattingWork) {
        EXPECT_EQ(-static_cast<::jxx::lang::jlong>(1), ::jxx::lang::Long::parseUnsignedLong(::jxx::NEW<::jxx::lang::String>("18446744073709551615")));
        EXPECT_EQ(-static_cast<::jxx::lang::jlong>(1), ::jxx::lang::Long::parseUnsignedLong(::jxx::NEW<::jxx::lang::String>("ffffffffffffffff"), 16));
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::NEW<::jxx::lang::String>("18446744073709551615"), ::jxx::lang::Long::toUnsignedString(-static_cast<::jxx::lang::jlong>(1))));
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::NEW<::jxx::lang::String>("ffffffffffffffff"), ::jxx::lang::Long::toUnsignedString(-static_cast<::jxx::lang::jlong>(1), 16)));
    }

    TEST(LongTest, FormattingSupportsCommonRadices) {
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::NEW<::jxx::lang::String>("123"), ::jxx::lang::String::valueOf(::jxx::lang::Long::toString(static_cast<::jxx::lang::jlong>(123)))));
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::NEW<::jxx::lang::String>("ff"), ::jxx::lang::String::valueOf(::jxx::lang::Long::toString(static_cast<::jxx::lang::jlong>(255), 16))));
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::NEW<::jxx::lang::String>("ff"), ::jxx::lang::Long::toHexString(static_cast<::jxx::lang::jlong>(255))));
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::NEW<::jxx::lang::String>("377"), ::jxx::lang::Long::toOctalString(static_cast<::jxx::lang::jlong>(255))));
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::NEW<::jxx::lang::String>("11111111"), ::jxx::lang::Long::toBinaryString(static_cast<::jxx::lang::jlong>(255))));
    }

    TEST(LongTest, EqualityHashAndComparisonWork) {
        ::jxx::Ptr<::jxx::lang::Long> value = ::jxx::lang::Long::valueOf(static_cast<::jxx::lang::jlong>(12));
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::NEW<::jxx::lang::String>("12"), ::jxx::lang::String::valueOf(value->toString())));
        EXPECT_TRUE(::jxx::util::Objects::equals(value, ::jxx::lang::String::valueOf(::jxx::lang::Long::valueOf(static_cast<::jxx::lang::jlong>(12)))));
        EXPECT_FALSE(::jxx::util::Objects::equals(value, ::jxx::lang::String::valueOf(::jxx::lang::Long::valueOf(static_cast<::jxx::lang::jlong>(13)))));
        EXPECT_EQ(12, value->hashCode());
        EXPECT_EQ(12, ::jxx::lang::Long::hashCode(static_cast<::jxx::lang::jlong>(12)));
        EXPECT_TRUE((value->compareTo(::jxx::lang::Long::valueOf(static_cast<::jxx::lang::jlong>(13))) < 0));
        EXPECT_EQ(0, ::jxx::lang::Long::compare(static_cast<::jxx::lang::jlong>(12), static_cast<::jxx::lang::jlong>(12)));
        EXPECT_TRUE((::jxx::lang::Long::compareUnsigned(-static_cast<::jxx::lang::jlong>(1), static_cast<::jxx::lang::jlong>(0)) > 0));
    }

    TEST(LongTest, SystemPropertyAccessUsesDefault) {
        ::jxx::Ptr<::jxx::lang::String> key = ::jxx::CONVERT<::jxx::Ptr<::jxx::lang::String>>("jdk8.lang.LongTest.number");
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
                EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::lang::String::valueOf(::jxx::lang::Long::valueOf(static_cast<::jxx::lang::jlong>(32))), ::jxx::lang::Long::getLong(key)));
                ::jxx::lang::System::clearProperty(key);
                EXPECT_EQ(::jxx::lang::Long::getLong(key), nullptr);
                EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::lang::String::valueOf(::jxx::lang::Long::valueOf(static_cast<::jxx::lang::jlong>(7))), ::jxx::lang::Long::getLong(key, static_cast<::jxx::lang::jlong>(7))));
                EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::lang::String::valueOf(::jxx::lang::Long::valueOf(static_cast<::jxx::lang::jlong>(9))), ::jxx::lang::Long::getLong(key, ::jxx::lang::Long::valueOf(static_cast<::jxx::lang::jlong>(9)))));
            } catch (...) {
                jxxFinally0.runAndDismiss();
                throw;
            }
        }
    }

    TEST(LongTest, BitOperationsMatchSixtyFourBitRepresentation) {
        EXPECT_EQ(64, ::jxx::lang::Long::numberOfLeadingZeros(static_cast<::jxx::lang::jlong>(0)));
        EXPECT_EQ(63, ::jxx::lang::Long::numberOfLeadingZeros(static_cast<::jxx::lang::jlong>(1)));
        EXPECT_EQ(64, ::jxx::lang::Long::numberOfTrailingZeros(static_cast<::jxx::lang::jlong>(0)));
        EXPECT_EQ(3, ::jxx::lang::Long::numberOfTrailingZeros(static_cast<::jxx::lang::jlong>(8)));
        EXPECT_EQ(static_cast<::jxx::lang::jlong>(8), ::jxx::lang::Long::highestOneBit(static_cast<::jxx::lang::jlong>(10)));
        EXPECT_EQ(static_cast<::jxx::lang::jlong>(2), ::jxx::lang::Long::lowestOneBit(static_cast<::jxx::lang::jlong>(10)));
        EXPECT_EQ(8, ::jxx::lang::Long::bitCount(static_cast<::jxx::lang::jlong>(0xF0F0)));
        EXPECT_EQ(static_cast<::jxx::lang::jlong>(0x23456789ABCDEF01), ::jxx::lang::Long::rotateLeft(static_cast<::jxx::lang::jlong>(0x123456789ABCDEF0), 4));
        EXPECT_EQ(static_cast<::jxx::lang::jlong>(0x0123456789ABCDEF), ::jxx::lang::Long::rotateRight(static_cast<::jxx::lang::jlong>(0x123456789ABCDEF0), 4));
        EXPECT_EQ(static_cast<::jxx::lang::jlong>(0x0807060504030201), ::jxx::lang::Long::reverseBytes(static_cast<::jxx::lang::jlong>(0x0102030405060708)));
        EXPECT_EQ(-1, ::jxx::lang::Long::signum(-static_cast<::jxx::lang::jlong>(100)));
        EXPECT_EQ(0, ::jxx::lang::Long::signum(static_cast<::jxx::lang::jlong>(0)));
        EXPECT_EQ(1, ::jxx::lang::Long::signum(static_cast<::jxx::lang::jlong>(100)));
    }

    TEST(LongTest, UnsignedArithmeticTreatsBitsAsUnsigned) {
        EXPECT_EQ(::jxx::lang::Long::MAX_VALUE, ::jxx::lang::Long::divideUnsigned(-static_cast<::jxx::lang::jlong>(1), static_cast<::jxx::lang::jlong>(2)));
        EXPECT_EQ(static_cast<::jxx::lang::jlong>(1), ::jxx::lang::Long::remainderUnsigned(-static_cast<::jxx::lang::jlong>(1), static_cast<::jxx::lang::jlong>(2)));
    }

    TEST(LongTest, ArithmeticHelpersWork) {
        EXPECT_EQ(static_cast<::jxx::lang::jlong>(20), ::jxx::lang::Long::sum(static_cast<::jxx::lang::jlong>(8), static_cast<::jxx::lang::jlong>(12)));
        EXPECT_EQ(static_cast<::jxx::lang::jlong>(12), ::jxx::lang::Long::max(static_cast<::jxx::lang::jlong>(8), static_cast<::jxx::lang::jlong>(12)));
        EXPECT_EQ(static_cast<::jxx::lang::jlong>(8), ::jxx::lang::Long::min(static_cast<::jxx::lang::jlong>(8), static_cast<::jxx::lang::jlong>(12)));
    }

} // namespace jxx::jdk8::lang
