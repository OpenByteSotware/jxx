#include <gtest/gtest.h>
#include <climits>
#include <limits>
#include "util/jxx.util.IntSummaryStatistics.h"
#include "util/jxx.util.LongSummaryStatistics.h"
#include "util/jxx.util.DoubleSummaryStatistics.h"
#include "lang/jxx.lang.NullPointerException.h"
TEST(JxxSummaryStatistics, EmptyDefaults)
{
	auto i = ::jxx::NEW<::jxx::util::IntSummaryStatistics>(); EXPECT_EQ(i->getCount(), 0); EXPECT_EQ(i->getSum(), 0); EXPECT_EQ(i->getMin(), INT_MAX); EXPECT_EQ(i->getMax(), INT_MIN); EXPECT_DOUBLE_EQ(i->getAverage(), 0.0);
}
TEST(JxxSummaryStatistics, AcceptAndCombine)
{
	auto a = ::jxx::NEW<::jxx::util::IntSummaryStatistics>(); 
	auto b = ::jxx::NEW<::jxx::util::IntSummaryStatistics>(); 
	a->accept(2); a->accept(4); b->accept(-1); a->combine(b); 
	EXPECT_EQ(a->getCount(), 3); EXPECT_EQ(a->getSum(), 5); 
	EXPECT_EQ(a->getMin(), -1); EXPECT_EQ(a->getMax(), 4);
}
TEST(JxxSummaryStatistics, LongAcceptsIntsAndLongs)
{
	auto s = ::jxx::NEW<::jxx::util::LongSummaryStatistics>(); 
	s->accept(static_cast<::jxx::lang::jint>(2)); 
	s->accept(static_cast<::jxx::lang::jlong>(5));
	EXPECT_EQ(s->getSum(), 7);
}
TEST(JxxSummaryStatistics, DoubleTracksSpecialValues)
{
	auto s = ::jxx::NEW<::jxx::util::DoubleSummaryStatistics>(); s->accept(1.0); s->accept(2.0); EXPECT_DOUBLE_EQ(s->getAverage(), 1.5);
	EXPECT_THROW(s->combine(nullptr), ::jxx::lang::NullPointerException);
}
