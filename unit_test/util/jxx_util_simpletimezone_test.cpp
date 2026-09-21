#include <gtest/gtest.h>
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.Date.h"
#include "util/jxx.util.SimpleTimeZone.h"
namespace {
TEST(SimpleTimeZoneParityTest, FixedOffsetZoneDoesNotUseDaylight){auto zone=::jxx::NEW<::jxx::util::SimpleTimeZone>(-28800000,::jxx::NEW<::jxx::lang::String>("PST"));EXPECT_EQ(-28800000,zone->getRawOffset());EXPECT_EQ(-28800000,zone->getOffset(0));EXPECT_FALSE(zone->useDaylightTime());EXPECT_EQ(0,zone->getDSTSavings());}
TEST(SimpleTimeZoneParityTest, NorthernHemisphereRulesApplyDaylightOffset){auto zone=::jxx::NEW<::jxx::util::SimpleTimeZone>(-28800000,::jxx::NEW<::jxx::lang::String>("PDT"),2,8,-1,7200000,10,1,-1,7200000);zone->setStartYear(2000);EXPECT_TRUE(zone->useDaylightTime());const ::jxx::lang::jlong january=1609459200000LL,july=1625097600000LL;EXPECT_EQ(-28800000,zone->getOffset(january));EXPECT_EQ(-25200000,zone->getOffset(july));EXPECT_TRUE(zone->inDaylightTime(::jxx::NEW<::jxx::util::Date>(july)));}
TEST(SimpleTimeZoneParityTest, SameRulesIgnoreIdentifierButEqualsDoesNot){auto first=::jxx::NEW<::jxx::util::SimpleTimeZone>(3600000,::jxx::NEW<::jxx::lang::String>("A"));auto second=::jxx::NEW<::jxx::util::SimpleTimeZone>(3600000,::jxx::NEW<::jxx::lang::String>("B"));EXPECT_TRUE(first->hasSameRules(second));EXPECT_FALSE(first->equals(::jxx::CAST<::jxx::lang::Object>(second)));}
}
