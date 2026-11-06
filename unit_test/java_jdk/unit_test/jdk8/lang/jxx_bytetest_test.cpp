#include <gtest/gtest.h>
#include <type_traits>
#include <utility>
#include "lang/jxx.lang.Byte.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.GenericConversion.h"
#include "lang/jxx.lang.JxxArray.h"
#include "lang/jxx.lang.NumberFormatException.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.System.h"
#include "util/jxx.util.Objects.h"

namespace jxx::jdk8::lang {
    TEST(ByteTest, ConstantsAreDocumented) {
        EXPECT_EQ(-128, ::jxx::lang::Byte::MIN_VALUE);
        EXPECT_EQ(127, ::jxx::lang::Byte::MAX_VALUE);
        EXPECT_EQ(8, ::jxx::lang::Byte::SIZE);
        EXPECT_EQ(1, ::jxx::lang::Byte::BYTES);
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::lang::Byte::TYPE, ::jxx::lang::Byte::TYPE));
    }

    TEST(ByteTest, ConstructorsAndNumberConversionsWork) {
        ::jxx::Ptr<::jxx::lang::Byte> value = ::jxx::NEW<::jxx::lang::Byte>(static_cast<::jxx::lang::jbyte>(127));
        EXPECT_EQ(static_cast<::jxx::lang::jbyte>(127), value->byteValue());
        EXPECT_EQ(static_cast<::jxx::lang::jshort>(127), value->shortValue());
        EXPECT_EQ(127, value->intValue());
        EXPECT_EQ(static_cast<::jxx::lang::jlong>(127), value->longValue());
        EXPECT_NEAR(static_cast<::jxx::lang::jfloat>(127.0), value->floatValue(), static_cast<::jxx::lang::jfloat>(0.0));
        EXPECT_NEAR(static_cast<::jxx::lang::jdouble>(127.0), value->doubleValue(), static_cast<::jxx::lang::jdouble>(0.0));
        EXPECT_EQ(static_cast<::jxx::lang::jbyte>(-12), (::jxx::NEW<::jxx::lang::Byte>(::jxx::NEW<::jxx::lang::String>("-12")))->byteValue());
    }

    TEST(ByteTest, ParsingSupportsDecimalAndRadix) {
        EXPECT_EQ(static_cast<::jxx::lang::jbyte>(42), ::jxx::lang::Byte::parseByte(::jxx::NEW<::jxx::lang::String>("42")));
        EXPECT_EQ(static_cast<::jxx::lang::jbyte>(-128), ::jxx::lang::Byte::parseByte(::jxx::NEW<::jxx::lang::String>("-128")));
        EXPECT_EQ(static_cast<::jxx::lang::jbyte>(127), ::jxx::lang::Byte::parseByte(::jxx::NEW<::jxx::lang::String>("7f"), 16));
        EXPECT_EQ(static_cast<::jxx::lang::jbyte>(10), ::jxx::lang::Byte::parseByte(::jxx::NEW<::jxx::lang::String>("1010"), 2));
    }

    TEST(ByteTest, ParsingRejectsOverflow) {
        EXPECT_THROW(
            {
                ::jxx::lang::Byte::parseByte(::jxx::NEW<::jxx::lang::String>("128"));
            },
            ::jxx::lang::NumberFormatException);
    }

    TEST(ByteTest, ValueOfAndDecodeWork) {
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::lang::Byte::valueOf(static_cast<::jxx::lang::jbyte>(10)), ::jxx::lang::Byte::valueOf(::jxx::NEW<::jxx::lang::String>("10"))));
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::lang::Byte::valueOf(static_cast<::jxx::lang::jbyte>(15)), ::jxx::lang::Byte::valueOf(::jxx::NEW<::jxx::lang::String>("f"), 16)));
        EXPECT_EQ(::jxx::lang::Byte::valueOf(static_cast<::jxx::lang::jbyte>(16)), ::jxx::lang::Byte::decode(::jxx::NEW<::jxx::lang::String>("0x10")));
        EXPECT_EQ(::jxx::lang::Byte::valueOf(static_cast<::jxx::lang::jbyte>(8)), ::jxx::lang::Byte::decode(::jxx::NEW<::jxx::lang::String>("010")));
        EXPECT_EQ(::jxx::lang::Byte::valueOf(static_cast<::jxx::lang::jbyte>(-16)), ::jxx::lang::Byte::decode(::jxx::NEW<::jxx::lang::String>("-0X10")));
    }

    TEST(ByteTest, StringEqualityHashAndComparisonWork) {
        ::jxx::Ptr<::jxx::lang::Byte> value = ::jxx::lang::Byte::valueOf(static_cast<::jxx::lang::jbyte>(12));
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::NEW<::jxx::lang::String>("12"), ::jxx::lang::String::valueOf(value->toString())));
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::NEW<::jxx::lang::String>("12"), ::jxx::lang::String::valueOf(::jxx::lang::Byte::toString(static_cast<::jxx::lang::jbyte>(12)))));
        EXPECT_TRUE(::jxx::util::Objects::equals(value, ::jxx::lang::Byte::valueOf(static_cast<::jxx::lang::jbyte>(12))));
        EXPECT_FALSE(::jxx::util::Objects::equals(value, ::jxx::lang::Byte::valueOf(static_cast<::jxx::lang::jbyte>(13))));
        EXPECT_EQ(12, value->hashCode());
        EXPECT_EQ(12, ::jxx::lang::Byte::hashCode(static_cast<::jxx::lang::jbyte>(12)));
        EXPECT_TRUE((value->compareTo(::jxx::lang::Byte::valueOf(static_cast<::jxx::lang::jbyte>(13))) < 0));
        EXPECT_TRUE((::jxx::lang::Byte::compare(static_cast<::jxx::lang::jbyte>(13), static_cast<::jxx::lang::jbyte>(12)) > 0));
        EXPECT_EQ(0, ::jxx::lang::Byte::compare(static_cast<::jxx::lang::jbyte>(12), static_cast<::jxx::lang::jbyte>(12)));
    }

    TEST(ByteTest, UnsignedConversionsZeroExtend) {
        EXPECT_EQ(255, ::jxx::lang::Byte::toUnsignedInt(static_cast<::jxx::lang::jbyte>(-1)));
        EXPECT_EQ(static_cast<::jxx::lang::jlong>(255), ::jxx::lang::Byte::toUnsignedLong(static_cast<::jxx::lang::jbyte>(-1)));
        EXPECT_EQ(128, ::jxx::lang::Byte::toUnsignedInt(::jxx::lang::Byte::MIN_VALUE));
    }

} // namespace jxx::jdk8::lang
