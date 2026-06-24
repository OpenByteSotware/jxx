#include <memory>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include "util/regex/jxx.util.regex.Matcher.h"
#include "util/regex/jxx.util.regex.MatchResult.h"
#include "util/regex/jxx.util.regex.Pattern.h"
#include "util/regex/jxx.util.regex.PatternSyntaxException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"

namespace {

using jxx::lang::String;
using jxx::util::regex::Matcher;
using jxx::util::regex::Pattern;
using jxx::util::regex::PatternSyntaxException;

static jxx::Ptr<String> S(const char* s) {
    return std::make_shared<String>(s);
}

TEST(PatternTest, CompileAndToString) {
    auto p = Pattern::compile(S("a+b"));
    ASSERT_NE(p, nullptr);
    EXPECT_EQ(p->pattern()->utf8(), "a+b");
    EXPECT_EQ(p->toString()->utf8(), "a+b");
    EXPECT_EQ(p->flags(), 0);
}

TEST(PatternTest, CompileRejectsInvalidRegex) {
    EXPECT_THROW((void)Pattern::compile(S("[abc")), PatternSyntaxException);
}

TEST(PatternTest, MatchesStaticMethod) {
    EXPECT_TRUE(Pattern::matches(S("a+b"), S("aaab")));
    EXPECT_FALSE(Pattern::matches(S("a+b"), S("aac")));
}

TEST(PatternTest, MatcherFindMatchesAndLookingAt) {
    auto pattern = Pattern::compile(S("ab"));
    auto matcher = pattern->matcher(S("zzabyyab"));

    EXPECT_TRUE(matcher->find());
    EXPECT_EQ(matcher->group()->utf8(), "ab");
    EXPECT_EQ(matcher->start(), 2);
    EXPECT_EQ(matcher->end(), 4);

    EXPECT_TRUE(matcher->find());
    EXPECT_EQ(matcher->start(), 6);
    EXPECT_EQ(matcher->end(), 8);

    auto m2 = pattern->matcher(S("abyy"));
    EXPECT_TRUE(m2->lookingAt());
    EXPECT_FALSE(m2->matches());

    auto m3 = pattern->matcher(S("ab"));
    EXPECT_TRUE(m3->matches());
}

TEST(PatternTest, MatcherGroupsAndGroupCount) {
    auto pattern = Pattern::compile(S("(ab)(cd)"));
    auto matcher = pattern->matcher(S("xxabcdyy"));
    ASSERT_TRUE(matcher->find());
    EXPECT_EQ(matcher->groupCount(), 2);
    EXPECT_EQ(matcher->group(0)->utf8(), "abcd");
    EXPECT_EQ(matcher->group(1)->utf8(), "ab");
    EXPECT_EQ(matcher->group(2)->utf8(), "cd");
}

TEST(PatternTest, MatcherResetAndFindFromOffset) {
    auto pattern = Pattern::compile(S("ab"));
    auto matcher = pattern->matcher(S("abxxab"));
    ASSERT_TRUE(matcher->find());
    EXPECT_EQ(matcher->start(), 0);
    ASSERT_TRUE(matcher->find(4));
    EXPECT_EQ(matcher->start(), 4);
    matcher->reset(S("zzab"));
    ASSERT_TRUE(matcher->find());
    EXPECT_EQ(matcher->start(), 2);
}

TEST(PatternTest, SplitAndQuote) {
    auto pattern = Pattern::compile(S(","));
    auto parts = pattern->split(S("a,b,c"));
    ASSERT_NE(parts, nullptr);
    ASSERT_EQ(parts->size(), 3);
    EXPECT_EQ((*parts)(0)->utf8(), "a");
    EXPECT_EQ((*parts)(1)->utf8(), "b");
    EXPECT_EQ((*parts)(2)->utf8(), "c");

    auto quoted = Pattern::quote(S("a+b?"));
    ASSERT_NE(quoted, nullptr);
    auto literal = Pattern::compile(quoted);
    EXPECT_TRUE(Pattern::matches(quoted, S("a+b?")));
    EXPECT_FALSE(Pattern::matches(quoted, S("aaab")));
    (void)literal;
}

TEST(PatternTest, ReplaceFunctions) {
    auto matcher = Pattern::compile(S("ab"))->matcher(S("zzabyyab"));
    EXPECT_EQ(matcher->replaceFirst(S("X"))->utf8(), "zzXyyab");
    EXPECT_EQ(matcher->replaceAll(S("X"))->utf8(), "zzXyyX");
}

TEST(PatternTest, QuoteReplacementEscapesDollarAndBackslash) {
    auto quoted = Matcher::quoteReplacement(S("$a\\b"));
    ASSERT_NE(quoted, nullptr);
    EXPECT_EQ(quoted->utf8(), "\\$a\\\\b");
}

TEST(PatternTest, NullPointerChecks) {
    EXPECT_THROW((void)Pattern::compile(nullptr), jxx::lang::NullPointerException);
    EXPECT_THROW((void)Pattern::matches(nullptr, S("x")), jxx::lang::NullPointerException);
    auto p = Pattern::compile(S("a"));
    EXPECT_THROW((void)p->matcher(nullptr), jxx::lang::NullPointerException);
    auto m = p->matcher(S("a"));
    EXPECT_THROW((void)m->replaceAll(nullptr), jxx::lang::NullPointerException);
    EXPECT_THROW((void)Matcher::quoteReplacement(nullptr), jxx::lang::NullPointerException);
}

} // anonymous namespace
