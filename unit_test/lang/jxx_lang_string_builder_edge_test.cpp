#include <gtest/gtest.h>
#include <limits>
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.StringBuilder.h"
#include "lang/jxx.lang.StringIndexOutOfBoundsException.h"
#include "lang/jxx.lang.buildin_array.h"

namespace {
using ::jxx::lang::String;
using ::jxx::lang::StringBuilder;
using ::jxx::lang::StringIndexOutOfBoundsException;

TEST(StringEdgeTest, FindsSupplementaryCodePointsByUtf16StartIndex) {
    const auto value = ::jxx::NEW<String>(std::u16string{u'A', 0xD83D, 0xDE00, u'B', 0xD83D, 0xDE00});
    EXPECT_EQ(1, value->indexOf(0x1F600));
    EXPECT_EQ(4, value->indexOf(0x1F600, 2));
    EXPECT_EQ(4, value->lastIndexOf(0x1F600));
    EXPECT_EQ(1, value->lastIndexOf(0x1F600, 3));
    EXPECT_EQ(-1, value->indexOf(0x110000));
}

TEST(StringEdgeTest, SplitHonorsPositiveZeroAndNegativeLimits) {
    const auto value = ::jxx::NEW<String>("a,b,,");
    const auto zero = value->split(::jxx::NEW<String>(","), 0);
    ASSERT_EQ(2U, zero->length);
    EXPECT_EQ("a", (*zero)[0]->utf8());
    EXPECT_EQ("b", (*zero)[1]->utf8());

    const auto positive = value->split(::jxx::NEW<String>(","), 2);
    ASSERT_EQ(2U, positive->length);
    EXPECT_EQ("a", (*positive)[0]->utf8());
    EXPECT_EQ("b,,", (*positive)[1]->utf8());

    const auto negative = value->split(::jxx::NEW<String>(","), -1);
    ASSERT_EQ(4U, negative->length);
    EXPECT_EQ("", (*negative)[3]->utf8());
}

TEST(StringEdgeTest, RegionMatchesLargeOffsetsReturnFalseWithoutOverflow) {
    const auto value = ::jxx::NEW<String>("abc");
    EXPECT_FALSE(value->regionMatches(std::numeric_limits<::jxx::lang::jint>::max(), value, 0, 1));
    EXPECT_FALSE(value->regionMatches(0, value, std::numeric_limits<::jxx::lang::jint>::max(), 1));
}

TEST(StringBuilderEdgeTest, ReversePreservesValidSurrogatePairs) {
    const auto builder = ::jxx::NEW<StringBuilder>(
        ::jxx::NEW<String>(std::u16string{u'A', 0xD83D, 0xDE00, u'B'}));
    builder->reverse();
    EXPECT_EQ(std::u16string({u'B', 0xD83D, 0xDE00, u'A'}), builder->toString()->utf16());
}

TEST(StringBuilderEdgeTest, DeleteRejectsNegativeEndAndClampsLargeEnd) {
    const auto builder = ::jxx::NEW<StringBuilder>(::jxx::NEW<String>("abcd"));
    EXPECT_THROW(builder->delete_(0, -1), StringIndexOutOfBoundsException);
    builder->delete_(2, 100);
    EXPECT_EQ("ab", builder->toString()->utf8());
}

TEST(StringBuilderEdgeTest, GetCharsChecksDestinationRangesWithoutOverflow) {
    const auto builder = ::jxx::NEW<StringBuilder>(::jxx::NEW<String>("abcd"));
    const auto destination = ::jxx::NEW<::jxx::lang::JxxArray<::jxx::lang::jchar, 1U>>(4U);
    EXPECT_THROW(builder->getChars(0, 4, destination,
        std::numeric_limits<::jxx::lang::jint>::max()), StringIndexOutOfBoundsException);
    builder->getChars(1, 3, destination, 1);
    EXPECT_EQ(static_cast<::jxx::lang::jchar>(u'b'), (*destination)[1]);
    EXPECT_EQ(static_cast<::jxx::lang::jchar>(u'c'), (*destination)[2]);
}
} // namespace
