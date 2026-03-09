#include <gtest/gtest.h>
#include <string>
#include "jxx.lang.String.h"

using namespace jxx::lang;

// NOTE: These tests assume String stores UTF-8 bytes and substring indices are byte-based
// We compute byte offsets using std::string so we cut only at codepoint boundaries.

TEST(StringUtf8Substring, MultibyteCharacters) {
    // "Grüße 🌍"  (G r ü ß e space globe)
    std::string utf8 = u8"Grüße 🌍";
    String s(utf8);

    // Byte offset of the space
    size_t space_pos = utf8.find(" ");
    ASSERT_NE(std::string::npos, space_pos);

    // substring(0, space) should be "Grüße"
    String left = s.substring(0, space_pos);
    EXPECT_EQ(std::string(u8"Grüße"), left.toStdString());

    // Extract the globe by bytes: find the start of "🌍" and use its UTF-8 length
    std::string globe = u8"🌍"; // 4 bytes
    size_t globe_pos = utf8.find(globe);
    ASSERT_NE(std::string::npos, globe_pos);
    String earth = s.substring(globe_pos, globe_pos + globe.size());
    EXPECT_EQ(globe, earth.toStdString());
}

TEST(StringUtf8Substring, EmojiAtEnd) {
    std::string utf8 = u8"Hi 😀"; // space + 4-byte emoji
    String s(utf8);
    std::string emoji = u8"😀";
    size_t pos = utf8.find(emoji);
    ASSERT_NE(std::string::npos, pos);
    String tail = s.substring(pos);
    EXPECT_EQ(emoji, tail.toStdString());
}
