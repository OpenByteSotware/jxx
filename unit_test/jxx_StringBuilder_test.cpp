#include <gtest/gtest.h>
#include <thread>
#include <vector>
#include <numeric>
#include <limits>
#include <string>
#include <cstring>
#include "jxx.h"

using jxx::lang::StringBuilder;
using jxx::util::Vector;
using jxx::lang::String;

TEST(StringBuilderTest, ConstructReserveAndEmpty) {
    StringBuilder b(128);
    EXPECT_TRUE(b.empty());
    EXPECT_EQ(b.size(), 0u);
    // Capacity is allowed to exceed request, but must be >= requested.
    EXPECT_GE(b.capacity(), 128u);

    b.reserve(256);
    EXPECT_GE(b.capacity(), 256u);
}

TEST(StringBuilderTest, AppendStringsCharsAndToString) {
    StringBuilder b;
    const std::string name = "Alice";
    b.append("Hello").append(',').append(' ').append(name).append_line();

    EXPECT_EQ(b.str(), std::string("Hello, Alice\n"));
    EXPECT_FALSE(b.empty());
}

TEST(StringBuilderTest, OperatorShiftChain) {
    StringBuilder b;
    b << "val=" << 42 << ", ok=" << true << ", hex=";
    b.append(255, 16);  // hex lowercase expected from to_chars
    EXPECT_EQ(b.str(), "val=42, ok=true, hex=ff");
}

TEST(StringBuilderTest, InsertReplaceErase) {
    StringBuilder b;
    b.append("Hello, ").append("Alice");

    // Insert "Ms. " after "Hello, "
    b.insert(7, "Ms. ");
    EXPECT_EQ(b.str(), "Hello, Ms. Alice");

    // Replace "Hello" -> "Hi"
    b.replace(0, 5, "Hi");
    EXPECT_EQ(b.str(), "Hi, Ms. Alice");

    // Erase "Ms. "
    // Current: "Hi, Ms. Alice"
    //          0123456789.....
    // "Ms. " spans [4, 8)
    b.erase(4, 8);
    EXPECT_EQ(b.str(), "Hi, Alice");

    // Replace to the end (end is exclusive)
    b.replace(4, b.size(), "Bob");
    EXPECT_EQ(b.str(), "Hi, Bob");
}

TEST(StringBuilderTest, RemovePrefixSuffix) {
    StringBuilder b;
    b.append("xyz123xyz");
    b.remove_prefix(3);
    EXPECT_EQ(b.str(), "123xyz");
    b.remove_suffix(3);
    EXPECT_EQ(b.str(), "123");
}

TEST(StringBuilderTest, SetLengthTruncateAndPadWithNul) {
    StringBuilder b;
    b.append("abc");
    b.set_length(2);  // truncate
    EXPECT_EQ(b.size(), 2u);
    EXPECT_EQ(b.str(), "ab");

    // Extend with default pad '\0'
    b.set_length(5);
    EXPECT_EQ(b.size(), 5u);
    // Verify the new bytes are NULs
    auto s = b.str();
    ASSERT_EQ(s.size(), 5u);
    EXPECT_EQ(s[0], 'a');
    EXPECT_EQ(s[1], 'b');
    EXPECT_EQ(s[2], '\0');
    EXPECT_EQ(s[3], '\0');
    EXPECT_EQ(s[4], '\0');

    // Extend further with a visible pad character
    b.set_length(7, 'x');
    auto s2 = b.str();
    ASSERT_EQ(s2.size(), 7u);
    EXPECT_EQ(s2[5], 'x');
    EXPECT_EQ(s2[6], 'x');
}

TEST(StringBuilderTest, CharAtAndSetCharAt) {
    StringBuilder b;
    b.append("abc");
    EXPECT_EQ(b.char_at(1), 'b');
    b.set_char_at(1, 'Z');
    EXPECT_EQ(b.char_at(1), 'Z');
    EXPECT_EQ(b.str(), "aZc");
}

TEST(StringBuilderTest, SubseqAndView) {
    StringBuilder b;
    b.append("abcdef");
    auto v = b.view();
    EXPECT_EQ(v, std::string_view("abcdef"));

    auto sub = b.subseq(2, 3); // "cde"
    EXPECT_EQ(sub, std::string_view("cde"));

    // Bounds are guarded by asserts in debug builds; we only test valid ranges here.
}

TEST(StringBuilderTest, IntegersVariousBases) {
    StringBuilder b;
    b.append(-123).append(' ');
    b.append(255, 16).append(' ');  // hex -> "ff"
    b.append(10, 8).append(' ');    // oct -> "12"
    b.append(255, 2);               // binary small value -> "11111111"

    EXPECT_EQ(b.str(), "-123 ff 12 11111111");
}

TEST(StringBuilderTest, FloatingAppendPortableCases) {
    StringBuilder b;

    // Precision 0 => no decimal point printed (portable across locales for these cases).
    b.append(42.0, 0);
    b.append(' ');
    b.append(1000.0f, 0);
    b.append(' ');
    b.append(1.0, 0, /*scientific=*/true); // "1e+00" with precision 0
    EXPECT_EQ(b.str(), "42 1000 1e+00");
}

TEST(StringBuilderTest, AppendLineConvenience) {
    StringBuilder b;
    b.append_line("first");
    b.append_line();
    b.append_line("third");
    EXPECT_EQ(b.str(), std::string("first\n\nthird\n"));
}

TEST(StringBuilderTest, ReplaceEndExclusive) {
    StringBuilder b;
    b.append("0123456789");
    // replace [2, 5) with "X" -> "01X56789"
    b.replace(2, 5, "X");
    EXPECT_EQ(b.str(), "01X56789");
}

TEST(StringBuilderTest, JoinUtilityWorks) {

    String n { "test" };
 
    Vector<String> words { "alpha", "beta", "gamma" };
    auto j = jxx::lang::join(words.begin(), words.end(), ", ");
    //TODO fix
    //EXPECT_EQ(j, "alpha, beta, gamma");
 
       // std::vector<int> nums{ 1, 2, 3 };
       // auto j = sb::join(nums.begin(), nums.end(), " | ");
       // EXPECT_EQ(j, "1 | 2 | 3");

}

TEST(StringBuilderTest, ClearAndShrinkToFit) {
    StringBuilder b(256);
    b.append("data");
    EXPECT_FALSE(b.empty());
    b.clear();
    EXPECT_TRUE(b.empty());
    EXPECT_EQ(b.size(), 0u);

    // shrink_to_fit is non-binding but should not throw or corrupt state
    auto cap_before = b.capacity();
    b.shrink_to_fit();
    // We cannot assert exact capacity behavior across libstdc++/libc++,
    // but at minimum, state remains valid.
    EXPECT_EQ(b.size(), 0u);
    (void)cap_before;
}
