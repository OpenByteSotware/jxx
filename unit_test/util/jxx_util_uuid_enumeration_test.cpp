#include <gtest/gtest.h>

#include <vector>

#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"
#include "util/jxx.util.NoSuchElementException.h"
#include "util/jxx.util.UUID.h"
#include "util/jxx.util.VectorEnumeration.h"

namespace {
using S = ::jxx::lang::String;

TEST(UUIDParityTest, ParsesAndFormatsStandardRepresentation) {
    const auto value = ::jxx::util::UUID::fromString(
        ::jxx::NEW<S>("123e4567-e89b-12d3-a456-426614174000"));
    EXPECT_TRUE(value->toString()->utf8() ==
        "123e4567-e89b-12d3-a456-426614174000");
    EXPECT_EQ(1, value->version());
    EXPECT_EQ(2, value->variant());
    EXPECT_EQ(0x2456, value->clockSequence());
    EXPECT_EQ(0x426614174000LL, value->node());
}

TEST(UUIDParityTest, RandomAndNameUUIDsHaveExpectedVersions) {
    const auto random = ::jxx::util::UUID::randomUUID();
    EXPECT_EQ(4, random->version());
    EXPECT_EQ(2, random->variant());

    auto bytes = ::jxx::NEW<::jxx::lang::ByteArrayType>(3);
    (*bytes)[0] = 'a'; (*bytes)[1] = 'b'; (*bytes)[2] = 'c';
    const auto named = ::jxx::util::UUID::nameUUIDFromBytes(bytes);
    EXPECT_EQ(3, named->version());
    EXPECT_EQ(2, named->variant());
    EXPECT_TRUE(named->toString()->utf8() ==
        "90015098-3cd2-3fb0-9696-3f7d28e17f72");
}

TEST(UUIDParityTest, EqualityHashAndComparisonUseBothHalves) {
    const auto left = ::jxx::NEW<::jxx::util::UUID>(1, 2);
    const auto same = ::jxx::NEW<::jxx::util::UUID>(1, 2);
    const auto greater = ::jxx::NEW<::jxx::util::UUID>(1, 3);
    EXPECT_TRUE(left->equals(::jxx::CAST<::jxx::lang::Object>(same)));
    EXPECT_EQ(left->hashCode(), same->hashCode());
    EXPECT_TRUE(left->compareTo(greater) < 0);
}

TEST(EnumerationParityTest, ExhaustionThrowsNoSuchElementException) {
    std::vector<::jxx::Ptr<S>> values{::jxx::NEW<S>("value")};
    auto enumeration = ::jxx::NEW<::jxx::util::VectorEnumeration<S>>(
        std::move(values));
    EXPECT_TRUE(enumeration->hasMoreElements());
    EXPECT_TRUE(enumeration->nextElement()->utf8() == "value");
    EXPECT_FALSE(enumeration->hasMoreElements());
    EXPECT_THROW(enumeration->nextElement(), ::jxx::util::NoSuchElementException);
}

} // namespace
