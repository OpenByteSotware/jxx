#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "util/jxx.util.StringTokenizer.h"

namespace {

TEST(StringTokenizerTest, AcceptsNativeDelimiterLiteralThroughJxxNew) {
    const auto source = ::jxx::NEW<::jxx::lang::String>("alpha beta");
    const auto tokenizer =
        ::jxx::NEW<::jxx::util::StringTokenizer>(source, " ");

    EXPECT_TRUE(tokenizer->nextToken()->equals(::jxx::NEW<::jxx::lang::String>("alpha")));
    EXPECT_TRUE(tokenizer->nextToken()->equals(::jxx::NEW<::jxx::lang::String>("beta")));
    EXPECT_FALSE(tokenizer->hasMoreTokens());
}

TEST(StringTokenizerTest, AcceptsLiteralWithReturnDelimiters) {
    const auto source = ::jxx::NEW<::jxx::lang::String>("a,b");
    const auto tokenizer =
        ::jxx::NEW<::jxx::util::StringTokenizer>(source, ",", true);

    EXPECT_TRUE(tokenizer->nextToken()->equals(::jxx::NEW<::jxx::lang::String>("a")));
    EXPECT_TRUE(tokenizer->nextToken()->equals(::jxx::NEW<::jxx::lang::String>(",")));
    EXPECT_TRUE(tokenizer->nextToken()->equals(::jxx::NEW<::jxx::lang::String>("b")));
}

TEST(StringTokenizerTest, AcceptsLiteralInNextToken) {
    const auto source = ::jxx::NEW<::jxx::lang::String>("left:right");
    const auto tokenizer =
        ::jxx::NEW<::jxx::util::StringTokenizer>(source);

    EXPECT_TRUE(tokenizer->nextToken(":")->equals(::jxx::NEW<::jxx::lang::String>("left")));
    EXPECT_TRUE(tokenizer->nextToken()->equals(::jxx::NEW<::jxx::lang::String>("right")));
}

} // namespace
