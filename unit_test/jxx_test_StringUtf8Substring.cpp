#include <gtest/gtest.h>
#include <string>
#include "jxx.lang.String.h"

using namespace jxx::lang;

// NOTE: These tests assume String stores UTF-8 bytes and substring indices are byte-based
// We compute byte offsets using std::string so we cut only at codepoint boundaries.

TEST(StringUtf8Substring, MultibyteCharacters)
{
    const auto value =
        ::jxx::NEW<::jxx::lang::String>(
            std::string(u8"Grüße 🌍"));

    ASSERT_EQ(8, value->length());

    const auto left = value->substring(0, 5);
    ASSERT_NE(nullptr, left);
    EXPECT_EQ(std::string(u8"Grüße"), left->utf8());

    const auto earth = value->substring(6, 8);
    ASSERT_NE(nullptr, earth);
    EXPECT_EQ(std::string(u8"🌍"), earth->utf8());
}
TEST(StringUtf8Substring, EmojiAtEnd) {
    std::string utf8 = u8"Hi 😀"; // space + 4-byte emoji
    String s(utf8);
    std::string emoji = u8"😀";
    size_t pos = utf8.find(emoji);
    ASSERT_NE(std::string::npos, pos);
    auto tail = s.substring(pos);
    //EXPECT_EQ(emoji, tail.toStdString());
}
