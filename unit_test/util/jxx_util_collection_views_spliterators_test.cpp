#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "util/function/jxx.util.function.Consumer.h"
#include "util/jxx.util.HashSet.h"
#include "util/jxx.util.Hashtable.h"
#include "util/jxx.util.MapEntry.h"
#include "util/jxx.util.Spliterator.h"

namespace {
using S = ::jxx::lang::String;

class CountingConsumer final
    : public ::jxx::util::function::Consumer<S> {
public:
    void acceptSuper(const ::jxx::Ptr<S> value) override {
        if (value != nullptr) ++count;
    }
    ::jxx::lang::jint count = 0;
};

TEST(AbstractSetSpliteratorParityTest, ReportsDistinctAndSizedAndTraverses) {
    const auto set = ::jxx::NEW<::jxx::util::HashSet<S>>();
    set->add(::jxx::NEW<S>("a"));
    set->add(::jxx::NEW<S>("b"));
    const auto spliterator = set->spliterator();
    EXPECT_TRUE(spliterator->hasCharacteristics(
        ::jxx::util::Spliterator<S>::DISTINCT));
    EXPECT_TRUE(spliterator->hasCharacteristics(
        ::jxx::util::Spliterator<S>::SIZED));
    EXPECT_EQ(2, spliterator->getExactSizeIfKnown());
    const auto consumer = ::jxx::NEW<CountingConsumer>();
    spliterator->forEachRemaining(
        ::jxx::CAST<::jxx::util::function::Consumer<S>>(consumer));
    EXPECT_EQ(2, consumer->count);
    EXPECT_EQ(0, spliterator->estimateSize());
}

TEST(HashtableViewSpliteratorParityTest, ExposesExpectedCharacteristics) {
    const auto table = ::jxx::NEW<::jxx::util::Hashtable<S, S>>();
    table->put(::jxx::NEW<S>("k1"), ::jxx::NEW<S>("v1"));
    table->put(::jxx::NEW<S>("k2"), ::jxx::NEW<S>("v2"));

    const auto keys = table->keySet()->spliterator();
    EXPECT_TRUE(keys->hasCharacteristics(
        ::jxx::util::Spliterator<S>::DISTINCT));
    EXPECT_TRUE(keys->hasCharacteristics(
        ::jxx::util::Spliterator<S>::SIZED));
    EXPECT_EQ(2, keys->getExactSizeIfKnown());

    const auto values = table->values()->spliterator();
    EXPECT_FALSE(values->hasCharacteristics(
        ::jxx::util::Spliterator<S>::DISTINCT));
    EXPECT_TRUE(values->hasCharacteristics(
        ::jxx::util::Spliterator<S>::SIZED));

    const auto entries = table->entrySet()->spliterator();
    EXPECT_TRUE(entries->hasCharacteristics(
        ::jxx::util::Spliterator<::jxx::util::MapEntry<S, S>>::DISTINCT));
    EXPECT_TRUE(entries->hasCharacteristics(
        ::jxx::util::Spliterator<::jxx::util::MapEntry<S, S>>::SIZED));
}

} // namespace
