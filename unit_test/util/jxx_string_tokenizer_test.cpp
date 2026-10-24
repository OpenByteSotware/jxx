#include <gtest/gtest.h>

#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.NoSuchElementException.h"
#include "util/jxx.util.StringTokenizer.h"

namespace {
using ::jxx::lang::String;
using ::jxx::util::StringTokenizer;

std::string text(const ::jxx::Ptr<String>& value) {
    return value->utf8();
}

TEST(StringTokenizerTest, UsesDefaultDelimiters) {
    auto tokenizer = ::jxx::NEW<StringTokenizer>(
        ::jxx::NEW<String>("this\tis\na\rtest\f"));

    EXPECT_EQ(4, tokenizer->countTokens());
    EXPECT_TRUE(tokenizer->hasMoreTokens());
    EXPECT_EQ("this", text(tokenizer->nextToken()));
    EXPECT_EQ("is", text(tokenizer->nextToken()));
    EXPECT_EQ("a", text(tokenizer->nextToken()));
    EXPECT_EQ("test", text(tokenizer->nextToken()));
    EXPECT_FALSE(tokenizer->hasMoreTokens());
    EXPECT_THROW(tokenizer->nextToken(), ::jxx::util::NoSuchElementException);
}

TEST(StringTokenizerTest, ReturnsEachDelimiterAsAToken) {
    auto tokenizer = ::jxx::NEW<StringTokenizer>(
        ::jxx::NEW<String>("a,,b"), ::jxx::NEW<String>(","), true);

    EXPECT_EQ(4, tokenizer->countTokens());
    EXPECT_EQ("a", text(tokenizer->nextToken()));
    EXPECT_EQ(",", text(tokenizer->nextToken()));
    EXPECT_EQ(",", text(tokenizer->nextToken()));
    EXPECT_EQ("b", text(tokenizer->nextToken()));
}

TEST(StringTokenizerTest, ChangesDelimitersForNextTokenAndFollowingCalls) {
    auto tokenizer = ::jxx::NEW<StringTokenizer>(
        ::jxx::NEW<String>("a,b;c"), ::jxx::NEW<String>(","));

    EXPECT_EQ("a", text(tokenizer->nextToken()));
    EXPECT_EQ("b", text(tokenizer->nextToken(::jxx::NEW<String>(";"))));
    EXPECT_EQ("c", text(tokenizer->nextToken()));
}

TEST(StringTokenizerTest, EnumerationMethodsMatchTokenMethods) {
    auto tokenizer = ::jxx::NEW<StringTokenizer>(
        ::jxx::NEW<String>("one two"));

    EXPECT_TRUE(tokenizer->hasMoreElements());
    auto first = ::jxx::CAST<String>(tokenizer->nextElement());
    ASSERT_NE(nullptr, first);
    EXPECT_EQ("one", text(first));
    EXPECT_EQ(1, tokenizer->countTokens());
}

TEST(StringTokenizerTest, SupportsSupplementaryDelimiterCodePoints) {
    auto tokenizer = ::jxx::NEW<StringTokenizer>(
        ::jxx::NEW<String>(u"left\U0001F600right"),
        ::jxx::NEW<String>(u"\U0001F600"),
        true);

    EXPECT_EQ("left", text(tokenizer->nextToken()));
    EXPECT_EQ("\xF0\x9F\x98\x80", text(tokenizer->nextToken()));
    EXPECT_EQ("right", text(tokenizer->nextToken()));
}

TEST(StringTokenizerTest, PreservesDocumentedNullBehavior) {
    EXPECT_THROW(
        ::jxx::NEW<StringTokenizer>(::jxx::Ptr<String>{}),
        ::jxx::lang::NullPointerException);

    auto tokenizer = ::jxx::NEW<StringTokenizer>(
        ::jxx::NEW<String>("value"), ::jxx::Ptr<String>{});
    EXPECT_THROW(
        tokenizer->hasMoreTokens(),
        ::jxx::lang::NullPointerException);
}
} // namespace
