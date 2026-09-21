#include <gtest/gtest.h>
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.StringBuffer.h"
#include "util/regex/jxx.util.regex.Matcher.h"
#include "util/regex/jxx.util.regex.Pattern.h"
namespace {
using S=::jxx::lang::String;
TEST(MatcherRegionParityTest, RestrictsSearchAndReportsAbsoluteIndices){auto pattern=::jxx::util::regex::Pattern::compile(::jxx::NEW<S>("cat"));auto matcher=pattern->matcher(::jxx::NEW<S>("cat dog cat"));matcher->region(4,11);EXPECT_EQ(4,matcher->regionStart());EXPECT_EQ(11,matcher->regionEnd());EXPECT_TRUE(matcher->find());EXPECT_EQ(8,matcher->start());EXPECT_EQ(11,matcher->end());EXPECT_TRUE(matcher->hitEnd());}
TEST(MatcherBoundsParityTest, StoresAnchoringAndTransparentPolicies){auto matcher=::jxx::util::regex::Pattern::compile(::jxx::NEW<S>("x"))->matcher(::jxx::NEW<S>("x"));EXPECT_TRUE(matcher->hasAnchoringBounds());EXPECT_FALSE(matcher->hasTransparentBounds());matcher->useAnchoringBounds(false)->useTransparentBounds(true);EXPECT_FALSE(matcher->hasAnchoringBounds());EXPECT_TRUE(matcher->hasTransparentBounds());}
TEST(MatcherReplacementParityTest, SupportsAppendReplacementAndTail){auto matcher=::jxx::util::regex::Pattern::compile(::jxx::NEW<S>("cat"))->matcher(::jxx::NEW<S>("cat and cat"));auto result=::jxx::NEW<::jxx::lang::StringBuffer>();while(matcher->find())matcher->appendReplacement(result,::jxx::NEW<S>("dog"));matcher->appendTail(result);EXPECT_TRUE(result->toString()->utf8()=="dog and dog");}
TEST(MatcherResetParityTest, RestoresFullRegionAndAppendPositionForNewInput){auto matcher=::jxx::util::regex::Pattern::compile(::jxx::NEW<S>("a"))->matcher(::jxx::NEW<S>("ba"));matcher->region(1,2);matcher->reset(::jxx::NEW<S>("aba"));EXPECT_EQ(0,matcher->regionStart());EXPECT_EQ(3,matcher->regionEnd());EXPECT_TRUE(matcher->find());EXPECT_EQ(0,matcher->start());}
}
