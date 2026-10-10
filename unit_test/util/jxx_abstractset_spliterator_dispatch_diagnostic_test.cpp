#include <gtest/gtest.h>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "util/function/jxx.util.function.Consumer.h"
#include "util/jxx.util.HashSet.h"
#include "util/jxx.util.Spliterator.h"

namespace {

using S = ::jxx::lang::String;

class CountingConsumer final
    : public ::jxx::lang::ClassBase<
          CountingConsumer,
          ::jxx::lang::Object,
          ::jxx::util::function::Consumer<S>> {
public:
    void accept(const ::jxx::Ptr<S>& value) override {
        ASSERT_NE(nullptr, value);
        ++count;
    }

    int count = 0;
};

TEST(AbstractSetSpliteratorDiagnosticTest, ConsumerCastPreservesVirtualDispatch) {
    const auto concrete = ::jxx::NEW<CountingConsumer>();
    const auto consumer = ::jxx::CAST<
        ::jxx::util::function::Consumer<S>>(concrete);

    ASSERT_NE(nullptr, consumer);
    consumer->accept(::jxx::NEW<S>("direct"));
    EXPECT_EQ(1, concrete->count);
}

TEST(AbstractSetSpliteratorDiagnosticTest, TryAdvanceInvokesConsumerAndUpdatesSize) {
    const auto set = ::jxx::NEW<::jxx::util::HashSet<S>>();
    EXPECT_TRUE(set->add(::jxx::NEW<S>("a")));
    EXPECT_TRUE(set->add(::jxx::NEW<S>("b")));

    const auto spliterator = set->spliterator();
    const auto concrete = ::jxx::NEW<CountingConsumer>();
    const auto consumer = ::jxx::CAST<
        ::jxx::util::function::Consumer<S>>(concrete);

    ASSERT_NE(nullptr, spliterator);
    ASSERT_NE(nullptr, consumer);
    EXPECT_EQ(2, spliterator->getExactSizeIfKnown());

    EXPECT_TRUE(spliterator->tryAdvance(consumer));
    EXPECT_EQ(1, concrete->count);
    EXPECT_EQ(1, spliterator->estimateSize());

    EXPECT_TRUE(spliterator->tryAdvance(consumer));
    EXPECT_EQ(2, concrete->count);
    EXPECT_EQ(0, spliterator->estimateSize());

    EXPECT_FALSE(spliterator->tryAdvance(consumer));
    EXPECT_EQ(2, concrete->count);
    EXPECT_EQ(0, spliterator->estimateSize());
}

TEST(AbstractSetSpliteratorDiagnosticTest, ForEachRemainingMatchesTryAdvanceDispatch) {
    const auto set = ::jxx::NEW<::jxx::util::HashSet<S>>();
    EXPECT_TRUE(set->add(::jxx::NEW<S>("a")));
    EXPECT_TRUE(set->add(::jxx::NEW<S>("b")));

    const auto spliterator = set->spliterator();
    const auto concrete = ::jxx::NEW<CountingConsumer>();
    const auto consumer = ::jxx::CAST<
        ::jxx::util::function::Consumer<S>>(concrete);

    ASSERT_NE(nullptr, spliterator);
    ASSERT_NE(nullptr, consumer);

    spliterator->forEachRemaining(consumer);

    EXPECT_EQ(2, concrete->count);
    EXPECT_EQ(0, spliterator->estimateSize());
    EXPECT_FALSE(spliterator->tryAdvance(consumer));
}

} // namespace
