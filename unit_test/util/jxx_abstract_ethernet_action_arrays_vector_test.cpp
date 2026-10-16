#include <gtest/gtest.h>

#include "lang/jxx.lang.buildin_array.h"
#include "util/jxx.util.Arrays.h"
#include "util/jxx.util.Vector.h"

TEST(AbstractEthernetActionParity, CopyOfRangeCopiesRequestedPayload) {
    auto source = ::jxx::NEW<::jxx::lang::ByteArrayType>(12);
    for (::jxx::lang::jint index = 0; index < source->length; ++index) {
        (*source)[index] = static_cast<::jxx::lang::jbyte>(index);
    }

    const auto result = ::jxx::util::Arrays::copyOfRange(
        source, 7, source->length);
    ASSERT_NE(nullptr, result);
    ASSERT_EQ(5, result->length);
    EXPECT_EQ(7, static_cast<int>((*result)[0]));
    EXPECT_EQ(11, static_cast<int>((*result)[4]));
}

TEST(AbstractEthernetActionParity, ByteArrayEqualityUsesContents) {
    auto left = ::jxx::NEW<::jxx::lang::ByteArrayType>(4);
    auto right = ::jxx::NEW<::jxx::lang::ByteArrayType>(4);
    for (::jxx::lang::jint index = 0; index < 4; ++index) {
        (*left)[index] = static_cast<::jxx::lang::jbyte>(index + 1);
        (*right)[index] = static_cast<::jxx::lang::jbyte>(index + 1);
    }
    EXPECT_TRUE(::jxx::util::Arrays::equals(left, right));
    (*right)[3] = 9;
    EXPECT_FALSE(::jxx::util::Arrays::equals(left, right));
    EXPECT_FALSE(::jxx::util::Arrays::equals(
        ::jxx::lang::ByteArray{}, right));
}

TEST(AbstractEthernetActionParity, VectorMaintainsLastFivePacketArrays) {
    using PacketArray = ::jxx::lang::ByteArrayType;
    auto values = ::jxx::NEW<::jxx::util::Vector<PacketArray>>();
    values->setSize(5);
    values->trimToSize();

    for (int value = 1; value <= 6; ++value) {
        if (values->size() >= 5) {
            values->remove(0);
        }
        auto packet = ::jxx::NEW<PacketArray>(4);
        (*packet)[0] = static_cast<::jxx::lang::jbyte>(value);
        values->add(packet);
    }

    ASSERT_EQ(5, values->size());
    EXPECT_EQ(2, static_cast<int>((*values->get(0))[0]));
    EXPECT_EQ(6, static_cast<int>((*values->get(4))[0]));
    values->clear();
    EXPECT_EQ(0, values->size());
}
