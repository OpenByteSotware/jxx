#include <gtest/gtest.h>
#include "util/jxx.util.SplittableRandom.h"
#include "lang/jxx.lang.IllegalArgumentException.h"

TEST(JxxSplittableRandom, SeededSequenceIsDeterministic)
{
	auto a = ::jxx::NEW<::jxx::util::SplittableRandom>(42); 
	auto b = ::jxx::NEW<::jxx::util::SplittableRandom>(42); 
	for (int i = 0; i < 20; ++i)EXPECT_EQ(a->nextLong(), b->nextLong());
}
TEST(JxxSplittableRandom, BoundsAreExclusive)
{
	auto r = ::jxx::NEW<::jxx::util::SplittableRandom>(7); for (int i = 0; i < 200; ++i) {
		auto v = r->nextInt(-3, 5); EXPECT_GE(v, -3); 
		EXPECT_LT(v, 5); auto d = r->nextDouble(2.0, 3.0);
		EXPECT_GE(d, 2.0); EXPECT_LT(d, 3.0);
	}
}
TEST(JxxSplittableRandom, SplitProducesUsableIndependentInstance)
{
	auto r = ::jxx::NEW<::jxx::util::SplittableRandom>(1); 
	auto s = r->split(); ASSERT_NE(s, nullptr); EXPECT_NE(r->nextLong(), s->nextLong());
}
TEST(JxxSplittableRandom, InvalidBoundsThrow)
{
	auto r = ::jxx::NEW<::jxx::util::SplittableRandom>(1);
	EXPECT_THROW(r->nextInt(0), ::jxx::lang::IllegalArgumentException);
	EXPECT_THROW(r->nextLong(4, 4), ::jxx::lang::IllegalArgumentException);
}
