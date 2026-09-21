#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.StringBuffer.h"

namespace {
using ::jxx::lang::String;
using ::jxx::lang::StringBuffer;

TEST(StringBufferParityTest, CapacityAndLengthFollowJavaGrowthRules) {
    const auto buffer = ::jxx::NEW<StringBuffer>();
    EXPECT_EQ(16, buffer->capacity());
    buffer->ensureCapacity(40);
    EXPECT_GE(buffer->capacity(), 40);
    buffer->setLength(4);
    EXPECT_EQ(4, buffer->length());
    EXPECT_EQ(0, buffer->charAt(0));
    buffer->trimToSize();
    EXPECT_EQ(buffer->length(), buffer->capacity());
}

TEST(StringBufferParityTest, ReversePreservesSurrogatePairs) {
    const auto buffer = ::jxx::NEW<StringBuffer>(
        ::jxx::NEW<String>(std::u16string{u'A', 0xD83D, 0xDE00, u'B'}));
    buffer->reverse();
    EXPECT_EQ(std::u16string({u'B', 0xD83D, 0xDE00, u'A'}),
              buffer->toString()->utf16());
}

TEST(StringBufferParityTest, AppendCodePointAndCodePointQueriesAgree) {
    const auto buffer = ::jxx::NEW<StringBuffer>();
    buffer->appendCodePoint(0x1F600);
    EXPECT_EQ(2, buffer->length());
    EXPECT_EQ(0x1F600, buffer->codePointAt(0));
    EXPECT_EQ(0x1F600, buffer->codePointBefore(2));
    EXPECT_EQ(1, buffer->codePointCount(0, 2));
}

} // namespace
