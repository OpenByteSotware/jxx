#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "util/jxx.util.Spliterator.h"
#include "util/jxx.util.Spliterators.h"
#include "util/function/jxx.util.function.Consumer.h"

namespace {
class CountingConsumer final
    : public ::jxx::lang::ClassBase<
          CountingConsumer,
          ::jxx::lang::Object,
          ::jxx::util::function::Consumer<::jxx::lang::String>> {
public:
    explicit CountingConsumer(int* count) : count_(count) {}

    void accept(
        const ::jxx::Ptr<::jxx::lang::String>&) override {
        ++*count_;
    }

private:
    int* count_;
};
} // namespace

TEST(JxxSpliteratorsEmptyParity, ReportsEmptySizedSpliterator) {
    auto spliterator = ::jxx::util::Spliterators::emptySpliterator<
        ::jxx::lang::String>();

    ASSERT_NE(spliterator, nullptr);
    EXPECT_EQ(spliterator->estimateSize(), 0);
    EXPECT_EQ(spliterator->getExactSizeIfKnown(), 0);
    EXPECT_TRUE(spliterator->hasCharacteristics(
        ::jxx::util::Spliterator<::jxx::lang::String>::SIZED));
    EXPECT_TRUE(spliterator->hasCharacteristics(
        ::jxx::util::Spliterator<::jxx::lang::String>::SUBSIZED));
    EXPECT_EQ(spliterator->trySplit(), nullptr);
}

TEST(JxxSpliteratorsEmptyParity, TraversalDoesNotInvokeConsumer) {
    auto spliterator = ::jxx::util::Spliterators::emptySpliterator<
        ::jxx::lang::String>();
    int count = 0;
    auto consumer = ::jxx::CAST<
        ::jxx::util::function::Consumer<::jxx::lang::String>>(
            ::jxx::NEW<CountingConsumer>(&count));

    EXPECT_FALSE(spliterator->tryAdvance(consumer));
    spliterator->forEachRemaining(consumer);
    EXPECT_EQ(count, 0);
}

TEST(JxxSpliteratorsEmptyParity, RejectsNullTraversalAction) {
    auto spliterator = ::jxx::util::Spliterators::emptySpliterator<
        ::jxx::lang::String>();
    EXPECT_THROW(
        spliterator->tryAdvance(nullptr),
        ::jxx::lang::NullPointerException);
}
