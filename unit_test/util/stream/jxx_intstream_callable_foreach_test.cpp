#include <gtest/gtest.h>

#include "util/stream/jxx.util.stream.IntStream.h"

namespace {

TEST(IntStreamCallableForEachTest, AcceptsCapturingLambda) {
    auto stream = ::jxx::util::stream::IntStream::of(
        static_cast<::jxx::lang::jint>(7));
    ::jxx::lang::jint observed = 0;

    stream->forEach([&](::jxx::lang::jint value) {
        observed = value;
    });

    EXPECT_EQ(7, observed);
}

TEST(IntStreamCallableForEachTest, AcceptsGenericLambda) {
    auto stream = ::jxx::util::stream::IntStream::range(1, 4);
    ::jxx::lang::jint sum = 0;

    stream->forEach([&](auto value) {
        sum += static_cast<::jxx::lang::jint>(value);
    });

    EXPECT_EQ(6, sum);
}

} // namespace
