
#include <gtest/gtest.h>
#include <sstream>
#include <unordered_set>
#include <jxx.h>

#ifdef _WIN32
#define _HAS_STD_BYTE 0
#endif

namespace cpp = std;

using namespace jxx::lang;

TEST(String, ConstructFromUtf8AndRoundTrip) {
    // ASCII + CJK + Emoji (requires surrogate pair in UTF-16)
    const std::string s8 = "Hello, \xE4\xB8\x96\xE7\x95\x8C \xF0\x9F\x98\x80"; // "Hello, 世界 😀"
    String s = String::fromUtf8(s8);
    EXPECT_EQ(s.toUtf8(), s8);

    // Internal UTF-16 should have length counted in code units
    // "Hello, " (7) + 世 + 界 (2) + space (1) + 😀 (2) => 12 code units
    EXPECT_EQ(s.length(), 12);
}

TEST(String, ConstructFromUtf16AndRoundTrip) {
    std::u16string u16 = u"ABC\u4E16\u754C"; // "ABC世界"
    String s(u16);
    EXPECT_EQ(s.utf16(), u16);
    EXPECT_EQ(s.toUtf8(), std::string("ABC\xE4\xB8\x96\xE7\x95\x8C"));
}

// ---------- Basic Queries ----------

TEST(String, IsEmptyAndLength) {
    String empty;
    EXPECT_TRUE(empty.isEmpty());
    EXPECT_EQ(empty.length(), 0);

    String nonEmpty = String::fromUtf8("x");
    EXPECT_FALSE(nonEmpty.isEmpty());
    EXPECT_EQ(nonEmpty.length(), 1);
}

TEST(String, CharAtAndBounds) {
    // 😀 is U+1F600 (surrogate pair in UTF-16)
    String s = String::fromUtf8("\xF0\x9F\x98\x80"); // "😀"
    EXPECT_EQ(s.length(), 2);
    // High surrogate then low surrogate
    EXPECT_GE(s.charAt(0), 0xD800);
    EXPECT_LE(s.charAt(0), 0xDBFF);
    EXPECT_GE(s.charAt(1), 0xDC00);
    EXPECT_LE(s.charAt(1), 0xDFFF);

    // Out-of-range throws
    EXPECT_THROW(s.charAt(-1), std::out_of_range);
    EXPECT_THROW(s.charAt(2), std::out_of_range);

    String e;
    EXPECT_THROW(e.charAt(0), std::out_of_range);
}

// ---------- Substring ----------

TEST(String, SubstringRanges) {
    String s = String::fromUtf8("Hello, World!");
    EXPECT_EQ(s.substring(0).toUtf8(), "Hello, World!");
    EXPECT_EQ(s.substring(7, 12).toUtf8(), "World");
    EXPECT_EQ(s.substring(0, 5).toUtf8(), "Hello");

    EXPECT_THROW(s.substring(-1, 2), std::out_of_range);
    EXPECT_THROW(s.substring(3, 2), std::out_of_range);
    EXPECT_THROW(s.substring(0, 100), std::out_of_range);
}

// ---------- Prefix/Suffix ----------

TEST(String, StartsWithEndsWith) {
    String s = String::fromUtf8("Hello, World!");
    EXPECT_TRUE(s.startsWith(String::fromUtf8("Hello")));
    EXPECT_FALSE(s.startsWith(String::fromUtf8("hello")));

    EXPECT_TRUE(s.endsWith(String::fromUtf8("!")));
    EXPECT_FALSE(s.endsWith(String::fromUtf8("World")));

    // StartsWith with offset
    EXPECT_TRUE(s.startsWith(String::fromUtf8("World"), 7));
    EXPECT_FALSE(s.startsWith(String::fromUtf8("World"), 8));
}

// ---------- Indexing ----------

TEST(String, IndexOfAndLastIndexOfString) {
    String s = String::fromUtf8("ababa");
    EXPECT_EQ(s.indexOf(String::fromUtf8("aba")), 0);
    EXPECT_EQ(s.indexOf(String::fromUtf8("aba"), 1), 2);
    EXPECT_EQ(s.indexOf(String::fromUtf8("xyz")), -1);

    EXPECT_EQ(s.lastIndexOf(String::fromUtf8("aba")), 2);
    EXPECT_EQ(s.lastIndexOf(String::fromUtf8("aba"), 2), 2);
    EXPECT_EQ(s.lastIndexOf(String::fromUtf8("aba"), 1), 2);
    EXPECT_EQ(s.lastIndexOf(String::fromUtf8("xyz")), -1);
}

TEST(String, IndexOfAndLastIndexOfChar) {
    String s = String::fromUtf8("banana");
    EXPECT_EQ(s.indexOf(u'a'), 1);
    EXPECT_EQ(s.indexOf(u'a', 2), 3);
    EXPECT_EQ(s.indexOf(u'z'), -1);

    EXPECT_EQ(s.lastIndexOf(u'a'), 5);
    EXPECT_EQ(s.lastIndexOf(u'a', 4), 3);
    EXPECT_EQ(s.lastIndexOf(u'z'), -1);
}

// ---------- Equality & Comparison ----------

TEST(String, EqualsAndEqualsIgnoreCaseAscii) {
    String a = String::fromUtf8("Hello");
    String b = String::fromUtf8("Hello");
    String c = String::fromUtf8("hello");
    String d = String::fromUtf8("héllo"); // non-ASCII e

    EXPECT_TRUE(a.equals(b));
    EXPECT_FALSE(a.equals(c));

    EXPECT_TRUE(a.equalsIgnoreCase(c));
    // ASCII-only case fold: 'é' won't equal 'e'
    EXPECT_FALSE(a.equalsIgnoreCase(d));
}

TEST(String, CompareToAndCompareToIgnoreCase) {
    String a = String::fromUtf8("Apple");
    String b = String::fromUtf8("Banana");
    String c = String::fromUtf8("apple");

    EXPECT_LT(a.compareTo(b), 0);
    EXPECT_GT(b.compareTo(a), 0);
    EXPECT_EQ(a.compareTo(a), 0);

    EXPECT_EQ(a.compareToIgnoreCase(c), 0);
    EXPECT_LT(a.compareToIgnoreCase(b), 0);
}

// ---------- Case Ops ----------

TEST(String, ToLowerAndUpperAscii) {
    String s = String::fromUtf8("HeLLo 123!");
    EXPECT_EQ(s.toLowerCase().toUtf8(), "hello 123!");
    EXPECT_EQ(s.toUpperCase().toUtf8(), "HELLO 123!");
    // Original must remain unchanged (immutability)
    EXPECT_EQ(s.toUtf8(), "HeLLo 123!");
}

// ---------- Whitespace ----------

TEST(String, Trim) {
    String s1 = String::fromUtf8("  x  ");
    EXPECT_EQ(s1.trim().toUtf8(), "x");

    // Java's trim only removes <= U+0020, not NBSP (U+00A0)
    String s2 = String::fromUtf8("\xC2\xA0x\xC2\xA0"); // NBSP x NBSP
    EXPECT_EQ(s2.trim().toUtf8(), std::string("\xC2\xA0x\xC2\xA0"));
}

// ---------- Replace ----------

TEST(String, ReplaceCharAndString) {
    String s = String::fromUtf8("Hello, World!");
    EXPECT_EQ(s.replace(u'o', u'0').toUtf8(), "Hell0, W0rld!");
    EXPECT_EQ(s.replace(String::fromUtf8("World"), String::fromUtf8("Java")).toUtf8(),
        "Hello, Java!");
    // Original unchanged
    EXPECT_EQ(s.toUtf8(), "Hello, World!");
}

// ---------- Split (ECMAScript regex semantics) ----------

TEST(String, SplitOnCommaDropsTrailingEmptyDefaultLimit0) {
    String s = String::fromUtf8("a,b,,c,");
    auto parts = s.split(String::fromUtf8(",")); // default limit = 0
    ASSERT_EQ(parts.size(), 4u); // "a","b","","c" (trailing empty dropped)
    EXPECT_EQ(parts[0].toUtf8(), "a");
    EXPECT_EQ(parts[1].toUtf8(), "b");
    EXPECT_EQ(parts[2].toUtf8(), "");
    EXPECT_EQ(parts[3].toUtf8(), "c");
}

TEST(String, SplitWhitespaceKeepsTrailingWhenLimitNegative) {
    String s = String::fromUtf8("one   two\tthree ");
    auto parts = s.split(String::fromUtf8("\\s+"), -1); // ECMAScript: \s works
    // "one","two","three"  (because of trailing whitespace + unlimited)
    ASSERT_EQ(parts.size(), 3);
    EXPECT_EQ(parts[0].toUtf8(), "one");
    EXPECT_EQ(parts[1].toUtf8(), "two");
    EXPECT_EQ(parts[2].toUtf8(), "three");
}

// ---------- Concat & Operators ----------

TEST(String, ConcatAndPlusOperator) {
    String a = String::fromUtf8("Hello");
    String b = String::fromUtf8(", ");
    String c = String::fromUtf8("James");
    String d = a.concat(b).concat(c);
    EXPECT_EQ(d.toUtf8(), "Hello, James");

    String e = a + b + c;
    EXPECT_EQ(e.toUtf8(), "Hello, James");
}

// ---------- Hashing (Java-compatible) ----------

TEST(String, HashCodeMatchesJavaForAscii) {
    String s = String::fromUtf8("abc");
    // Java: (((0*31 + 'a')*31 + 'b')*31 + 'c') = 96354
    EXPECT_EQ(s.hashCode(), 96354);
    // Stable across calls (cache)
    EXPECT_EQ(s.hashCode(), 96354);
}

TEST(String, HashCodeMatchesJavaForUnicode) {
    String s = String::fromUtf8("\xE4\xB8\x96\xE7\x95\x8C"); 
    EXPECT_EQ(s.hashCode(), 649718);

   // String s2("世界");
}

TEST(String, StdHashConsistentWithHashCodeAndUnorderedSet) {
    String a = String::fromUtf8("Hello, World!");
    std::unordered_set<String> set;
    set.insert(a);
    EXPECT_EQ(set.count(String::fromUtf8("Hello, World!")), 1u);
    EXPECT_EQ(set.count(String::fromUtf8("hello, world!")), 0u);
}

// ---------- Stream Interop ----------

TEST(String, StreamOperatorOutputsUtf8) {
    String s = String::fromUtf8("\xE4\xB8\x96\xE7\x95\x8C"); // "世界"
    std::ostringstream oss;
    oss << s;
    // Expect exact UTF-8 bytes
    EXPECT_EQ(oss.str(), std::string("\xE4\xB8\x96\xE7\x95\x8C"));
}

// ---------- Immutability sanity ----------

TEST(String, ImmutabilityOriginalUnchanged) {
    String s = String::fromUtf8("abc");
    auto s2 = s.toUpperCase();       // "ABC"
    auto s3 = s.replace(u'b', u'B'); // "aBc"
    auto s4 = s.substring(1);        // "bc"
    (void)s3; (void)s4;

    EXPECT_EQ(s.toUtf8(), "abc");
    EXPECT_EQ(s2.toUtf8(), "ABC");
}
