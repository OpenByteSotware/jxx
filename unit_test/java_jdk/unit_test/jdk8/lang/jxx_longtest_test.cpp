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

namespace jxx::jdk8::lang {
    TEST(LongTest, ConstantsAreDocumented) {
        EXPECT_EQ(::jxx::lang::Long::MIN_VALUE, (9223372036854775807L - 1L));
        EXPECT_EQ(::jxx::lang::Long::MAX_VALUE, 9223372036854775807L);
        EXPECT_EQ(64, ::jxx::lang::Long::SIZE);
        EXPECT_EQ(8, ::jxx::lang::Long::BYTES);
        EXPECT_EQ(ClassReference(postfix_operators=[], prefix_operators=[], qualifier=None, selectors=[], type=BasicType(dimensions=[], name=long)), ::jxx::lang::Long::TYPE);
    }

    TEST(LongTest, ConstructorsAndNumberConversionsWork) {
        ::jxx::Ptr<::jxx::lang::Long> value = ::jxx::NEW<::jxx::lang::Long>(258L);
        EXPECT_EQ(static_cast<::jxx::lang::jbyte>(2), value->byteValue());
        EXPECT_EQ(static_cast<::jxx::lang::jshort>(258), value->shortValue());
        EXPECT_EQ(258, value->intValue());
        EXPECT_EQ(258L, value->longValue());
        EXPECT_NEAR(258.0f, value->floatValue(), 0.0f);
        EXPECT_NEAR(258.0d, value->doubleValue(), 0.0d);
        EXPECT_EQ(42L, ::jxx::NEW<::jxx::lang::Long>(::jxx::NEW<::jxx::lang::String>("42"))->longValue());
    }

    TEST(LongTest, ParsingValueOfAndDecodeWork) {
        EXPECT_EQ(123L, ::jxx::lang::Long::parseLong(::jxx::NEW<::jxx::lang::String>("123")));
        EXPECT_EQ(255L, ::jxx::lang::Long::parseLong(::jxx::NEW<::jxx::lang::String>("ff"), 16));
        EXPECT_EQ(10L, ::jxx::lang::Long::parseLong(::jxx::NEW<::jxx::lang::String>("-1010"), 2));
        EXPECT_EQ(123L, ::jxx::lang::Long::valueOf(::jxx::NEW<::jxx::lang::String>("123"))->longValue());
        EXPECT_EQ(255L, ::jxx::lang::Long::valueOf(::jxx::NEW<::jxx::lang::String>("ff"), 16)->longValue());
        EXPECT_EQ(16L, ::jxx::lang::Long::decode(::jxx::NEW<::jxx::lang::String>("0x10"))->longValue());
        EXPECT_EQ(8L, ::jxx::lang::Long::decode(::jxx::NEW<::jxx::lang::String>("010"))->longValue());
    }

    TEST(LongTest, ParseLongRejectsOverflow) {
        ::jxx::lang::Long::parseLong(::jxx::NEW<::jxx::lang::String>("9223372036854775808"));
    }

    TEST(LongTest, UnsignedParsingAndFormattingWork) {
        EXPECT_EQ(1L, ::jxx::lang::Long::parseUnsignedLong(::jxx::NEW<::jxx::lang::String>("18446744073709551615")));
        EXPECT_EQ(1L, ::jxx::lang::Long::parseUnsignedLong(::jxx::NEW<::jxx::lang::String>("ffffffffffffffff"), 16));
        assertEquals(::jxx::NEW<::jxx::lang::String>("18446744073709551615"), ::jxx::lang::Long::toUnsignedString(1L));
        assertEquals(::jxx::NEW<::jxx::lang::String>("ffffffffffffffff"), ::jxx::lang::Long::toUnsignedString(1L, 16));
    }

    TEST(LongTest, FormattingSupportsCommonRadices) {
        assertEquals(::jxx::NEW<::jxx::lang::String>("123"), ::jxx::lang::Long::toString(123L));
        assertEquals(::jxx::NEW<::jxx::lang::String>("ff"), ::jxx::lang::Long::toString(255L, 16));
        assertEquals(::jxx::NEW<::jxx::lang::String>("ff"), ::jxx::lang::Long::toHexString(255L));
        assertEquals(::jxx::NEW<::jxx::lang::String>("377"), ::jxx::lang::Long::toOctalString(255L));
        assertEquals(::jxx::NEW<::jxx::lang::String>("11111111"), ::jxx::lang::Long::toBinaryString(255L));
    }

    TEST(LongTest, EqualityHashAndComparisonWork) {
        ::jxx::Ptr<::jxx::lang::Long> value = ::jxx::lang::Long::valueOf(12L);
        assertEquals(::jxx::NEW<::jxx::lang::String>("12"), value->toString());
        EXPECT_EQ(value, ::jxx::lang::Long::valueOf(12L));
        EXPECT_NE(value, ::jxx::lang::Long::valueOf(13L));
        EXPECT_EQ(12, value->hashCode());
        EXPECT_EQ(12, ::jxx::lang::Long::hashCode(12L));
        EXPECT_TRUE((value->compareTo(::jxx::lang::Long::valueOf(13L)) < 0));
        EXPECT_EQ(0, ::jxx::lang::Long::compare(12L, 12L));
        EXPECT_TRUE((::jxx::lang::Long::compareUnsigned(1L, 0L) > 0));
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
                EXPECT_EQ(::jxx::lang::Long::valueOf(32L), ::jxx::lang::Long::getLong(key));
                ::jxx::lang::System::clearProperty(key);
                EXPECT_EQ(::jxx::lang::Long::getLong(key), nullptr);
                EXPECT_EQ(::jxx::lang::Long::valueOf(7L), ::jxx::lang::Long::getLong(key, 7L));
                EXPECT_EQ(::jxx::lang::Long::valueOf(9L), ::jxx::lang::Long::getLong(key, ::jxx::lang::Long::valueOf(9L)));
            } catch (...) {
                jxxFinally0.runAndDismiss();
                throw;
            }
        }
    }

    TEST(LongTest, BitOperationsMatchSixtyFourBitRepresentation) {
        EXPECT_EQ(64, ::jxx::lang::Long::numberOfLeadingZeros(0L));
        EXPECT_EQ(63, ::jxx::lang::Long::numberOfLeadingZeros(1L));
        EXPECT_EQ(64, ::jxx::lang::Long::numberOfTrailingZeros(0L));
        EXPECT_EQ(3, ::jxx::lang::Long::numberOfTrailingZeros(8L));
        EXPECT_EQ(8L, ::jxx::lang::Long::highestOneBit(10L));
        EXPECT_EQ(2L, ::jxx::lang::Long::lowestOneBit(10L));
        EXPECT_EQ(8, ::jxx::lang::Long::bitCount(0xF0F0L));
        EXPECT_EQ(0x23456789ABCDEF01L, ::jxx::lang::Long::rotateLeft(0x123456789ABCDEF0L, 4));
        EXPECT_EQ(0x0123456789ABCDEFL, ::jxx::lang::Long::rotateRight(0x123456789ABCDEF0L, 4));
        EXPECT_EQ(0x0807060504030201L, ::jxx::lang::Long::reverseBytes(0x0102030405060708L));
        EXPECT_EQ(1, ::jxx::lang::Long::signum(100L));
        EXPECT_EQ(0, ::jxx::lang::Long::signum(0L));
        EXPECT_EQ(1, ::jxx::lang::Long::signum(100L));
    }

    TEST(LongTest, UnsignedArithmeticTreatsBitsAsUnsigned) {
        EXPECT_EQ(::jxx::lang::Long::MAX_VALUE, ::jxx::lang::Long::divideUnsigned(1L, 2L));
        EXPECT_EQ(1L, ::jxx::lang::Long::remainderUnsigned(1L, 2L));
    }

    TEST(LongTest, ArithmeticHelpersWork) {
        EXPECT_EQ(20L, ::jxx::lang::Long::sum(8L, 12L));
        EXPECT_EQ(12L, ::jxx::lang::Long::max(8L, 12L));
        EXPECT_EQ(8L, ::jxx::lang::Long::min(8L, 12L));
    }

} // namespace jxx::jdk8::lang
