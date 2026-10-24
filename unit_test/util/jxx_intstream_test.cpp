#include <gtest/gtest.h>

#include "lang/jxx.lang.IllegalStateException.h"
#include "util/jxx.util.IntStream.h"
#include "util/jxx.util.OptionalDouble.h"
#include "util/jxx.util.OptionalInt.h"

namespace {

::jxx::lang::IntArray values(
    std::initializer_list<::jxx::lang::jint> source) {
    return ::jxx::NEW<::jxx::lang::IntArrayType>(source);
}

TEST(IntStreamTest, DistinctPreservesEncounterOrder) {
    auto result = ::jxx::util::IntStream::of(
        values({3, 1, 3, 2, 1, 2}))
        ->distinct()
        ->toArray();

    ASSERT_EQ(3U, result->length);
    EXPECT_EQ(3, (*result)[0]);
    EXPECT_EQ(1, (*result)[1]);
    EXPECT_EQ(2, (*result)[2]);
}

TEST(IntStreamTest, RangeSortSkipLimitAndSumCompose) {
    auto result = ::jxx::util::IntStream::rangeClosed(1, 8)
        ->skip(2)
        ->limit(4)
        ->sum();

    EXPECT_EQ(18, result);
}

TEST(IntStreamTest, EmptyTerminalOperationsMatchExpectedValues) {
    EXPECT_EQ(0, ::jxx::util::IntStream::empty()->sum());
    EXPECT_EQ(0, ::jxx::util::IntStream::empty()->count());
    EXPECT_FALSE(::jxx::util::IntStream::empty()->min()->isPresent());
    EXPECT_FALSE(::jxx::util::IntStream::empty()->max()->isPresent());
    EXPECT_FALSE(::jxx::util::IntStream::empty()->average()->isPresent());
}

TEST(IntStreamTest, ToArrayReturnsIndependentStorage) {
    auto source = values({4, 5, 6});
    auto result = ::jxx::util::IntStream::of(source)->toArray();
    (*source)[0] = 99;
    EXPECT_EQ(4, (*result)[0]);
}

TEST(IntStreamTest, StreamCannotBeReusedAfterTerminalOperation) {
    auto stream = ::jxx::util::IntStream::of(values({1, 2, 3}));
    EXPECT_EQ(6, stream->sum());
    EXPECT_THROW(stream->count(), ::jxx::lang::IllegalStateException);
}

} // namespace
