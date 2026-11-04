#include <gtest/gtest.h>
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.GenericConversion.h"
#include "lang/jxx.lang.JxxArray.h"
#include "lang/jxx.lang.NumberFormatException.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.Short.h"
#include "lang/jxx.lang.System.h"

namespace jxx::jdk8::lang {
    TEST(ShortTest, ConstantsAreDocumented) {
        EXPECT_EQ(32768, ::jxx::lang::Short::MIN_VALUE);
        EXPECT_EQ(32767, ::jxx::lang::Short::MAX_VALUE);
        EXPECT_EQ(16, ::jxx::lang::Short::SIZE);
        EXPECT_EQ(2, ::jxx::lang::Short::BYTES);
        EXPECT_EQ(ClassReference(postfix_operators=[], prefix_operators=[], qualifier=None, selectors=[], type=BasicType(dimensions=[], name=short)), ::jxx::lang::Short::TYPE);
    }

    TEST(ShortTest, ConstructorsAndNumberConversionsWork) {
        ::jxx::Ptr<::jxx::lang::Short> value = ::jxx::NEW<::jxx::lang::Short>(static_cast<::jxx::lang::jshort>(12345));
        EXPECT_EQ(static_cast<::jxx::lang::jbyte>(57), value->byteValue());
        EXPECT_EQ(static_cast<::jxx::lang::jshort>(12345), value->shortValue());
        EXPECT_EQ(12345, value->intValue());
        EXPECT_EQ(12345L, value->longValue());
        EXPECT_NEAR(12345.0f, value->floatValue(), 0.0f);
        EXPECT_NEAR(12345.0d, value->doubleValue(), 0.0d);
        EXPECT_EQ(static_cast<::jxx::lang::jshort>(12), ::jxx::NEW<::jxx::lang::Short>(::jxx::NEW<::jxx::lang::String>("-12")));
    }

    TEST(ShortTest, ParsingSupportsDecimalAndRadix) {
        EXPECT_EQ(static_cast<::jxx::lang::jshort>(1234), ::jxx::lang::Short::parseShort(::jxx::NEW<::jxx::lang::String>("1234")));
        EXPECT_EQ(static_cast<::jxx::lang::jshort>(32767), ::jxx::lang::Short::parseShort(::jxx::NEW<::jxx::lang::String>("7fff"), 16));
        EXPECT_EQ(static_cast<::jxx::lang::jshort>(10), ::jxx::lang::Short::parseShort(::jxx::NEW<::jxx::lang::String>("1010"), 2));
    }

    TEST(ShortTest, ParsingRejectsOverflow) {
        ::jxx::lang::Short::parseShort(::jxx::NEW<::jxx::lang::String>("32768"));
    }

    TEST(ShortTest, ValueOfAndDecodeWork) {
        EXPECT_EQ(::jxx::lang::Short::valueOf(static_cast<::jxx::lang::jshort>(10)), ::jxx::lang::Short::valueOf(::jxx::NEW<::jxx::lang::String>("10")));
        EXPECT_EQ(::jxx::lang::Short::valueOf(static_cast<::jxx::lang::jshort>(15)), ::jxx::lang::Short::valueOf(::jxx::NEW<::jxx::lang::String>("f"), 16));
        EXPECT_EQ(::jxx::lang::Short::valueOf(static_cast<::jxx::lang::jshort>(16)), ::jxx::lang::Short::decode(::jxx::NEW<::jxx::lang::String>("0x10")));
        EXPECT_EQ(::jxx::lang::Short::valueOf(static_cast<::jxx::lang::jshort>(8)), ::jxx::lang::Short::decode(::jxx::NEW<::jxx::lang::String>("010")));
    }

    TEST(ShortTest, StringEqualityHashAndComparisonWork) {
        ::jxx::Ptr<::jxx::lang::Short> value = ::jxx::lang::Short::valueOf(static_cast<::jxx::lang::jshort>(1200));
        assertEquals(::jxx::NEW<::jxx::lang::String>("1200"), value->toString());
        assertEquals(::jxx::NEW<::jxx::lang::String>("1200"), ::jxx::lang::Short::toString(static_cast<::jxx::lang::jshort>(1200)));
        EXPECT_EQ(value, ::jxx::lang::Short::valueOf(static_cast<::jxx::lang::jshort>(1200)));
        EXPECT_NE(value, ::jxx::lang::Short::valueOf(static_cast<::jxx::lang::jshort>(1201)));
        EXPECT_EQ(1200, value->hashCode());
        EXPECT_EQ(1200, ::jxx::lang::Short::hashCode(static_cast<::jxx::lang::jshort>(1200)));
        EXPECT_TRUE((value->compareTo(::jxx::lang::Short::valueOf(static_cast<::jxx::lang::jshort>(1201))) < 0));
        EXPECT_TRUE((::jxx::lang::Short::compare(static_cast<::jxx::lang::jshort>(1201), static_cast<::jxx::lang::jshort>(1200)) > 0));
    }

    TEST(ShortTest, ReverseBytesSwapsBothBytes) {
        EXPECT_EQ(static_cast<::jxx::lang::jshort>(0x3412), ::jxx::lang::Short::reverseBytes(static_cast<::jxx::lang::jshort>(0x1234)));
    }

    TEST(ShortTest, UnsignedConversionsZeroExtend) {
        EXPECT_EQ(65535, ::jxx::lang::Short::toUnsignedInt(static_cast<::jxx::lang::jshort>(1)));
        EXPECT_EQ(65535L, ::jxx::lang::Short::toUnsignedLong(static_cast<::jxx::lang::jshort>(1)));
        EXPECT_EQ(32768, ::jxx::lang::Short::toUnsignedInt(::jxx::lang::Short::MIN_VALUE));
    }

} // namespace jxx::jdk8::lang
