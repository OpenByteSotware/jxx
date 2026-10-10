#include <gtest/gtest.h>

#include "util/jxx.util.BitSet.h"
#include "lang/jxx.lang.NullPointerException.h"

TEST(JxxBitSetArrayParityTest, ByteArrayUsesLittleEndianBitIndexMapping) {
    auto bytes = ::jxx::NEW<::jxx::lang::ByteArrayType>(2);
    (*bytes)[0] = static_cast<::jxx::lang::jbyte>(0x81);
    (*bytes)[1] = static_cast<::jxx::lang::jbyte>(0x02);

    const auto bits = ::jxx::util::BitSet::valueOf(bytes);
    EXPECT_TRUE(bits->get(0));
    EXPECT_TRUE(bits->get(7));
    EXPECT_TRUE(bits->get(9));
    EXPECT_EQ(bits->toByteArray()->length, 2);
}

TEST(JxxBitSetArrayParityTest, LongArrayRoundTripsAndTrimsTrailingZeros) {
    auto longs = ::jxx::NEW<::jxx::lang::LongArrayType>(3);
    (*longs)[0] = 1;
    (*longs)[1] = static_cast<::jxx::lang::jlong>(1ULL << 63U);
    (*longs)[2] = 0;

    const auto bits = ::jxx::util::BitSet::valueOf(longs);
    EXPECT_TRUE(bits->get(0));
    EXPECT_TRUE(bits->get(127));

    const auto roundTrip = bits->toLongArray();
    ASSERT_EQ(roundTrip->length, 2);
    EXPECT_EQ((*roundTrip)[0], 1);
    EXPECT_EQ(static_cast<std::uint64_t>((*roundTrip)[1]), 1ULL << 63U);
}

TEST(JxxBitSetArrayParityTest, NullArraysAreRejected) {
    EXPECT_THROW(::jxx::util::BitSet::valueOf(::jxx::lang::ByteArray{}),
                 ::jxx::lang::NullPointerException);
    EXPECT_THROW(::jxx::util::BitSet::valueOf(::jxx::lang::LongArray{}),
                 ::jxx::lang::NullPointerException);
}
