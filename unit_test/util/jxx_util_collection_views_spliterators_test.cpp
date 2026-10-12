#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "util/function/jxx.util.function.Consumer.h"
#include "util/jxx.util.HashSet.h"
#include "util/jxx.util.Hashtable.h"
#include "util/jxx.util.MapEntry.h"
#include "util/jxx.util.Spliterator.h"

namespace XML_Test {
using S = ::jxx::lang::String;

class CountingConsumer final
    : public ::jxx::util::function::Consumer<S> {
public:
    void acceptSuper(const ::jxx::Ptr<S>& value) override {
        if (value != nullptr) ++count;
    }
    ::jxx::lang::jint count = 0;
};

namespace
{

    using S = ::jxx::lang::String;

    class AbstractSetCountingConsumer2 final
        : public ::jxx::lang::ClassBase<
        AbstractSetCountingConsumer2,
        ::jxx::lang::Object,
        ::jxx::util::function::Consumer<S>> {
    public:
        void accept(
            const ::jxx::Ptr<S>& value) override
        {
            (void)value;
            ++count;
        }

        ::jxx::lang::jint count = 0;
    };

} // namespace

TEST(
    AbstractSetSpliteratorParityTest2,
    ReportsDistinctAndSizedAndTraverses)
{
    const auto set =
        ::jxx::NEW<::jxx::util::HashSet<S>>();

    EXPECT_TRUE(set->add(::jxx::NEW<S>("a")));
    EXPECT_TRUE(set->add(::jxx::NEW<S>("b")));

    const auto spliterator =
        set->spliterator();

    ASSERT_NE(nullptr, spliterator);

    EXPECT_TRUE(spliterator->hasCharacteristics(
        ::jxx::util::Spliterator<S>::DISTINCT));

    EXPECT_TRUE(spliterator->hasCharacteristics(
        ::jxx::util::Spliterator<S>::SIZED));

    EXPECT_EQ(
        2,
        spliterator->getExactSizeIfKnown());

    const auto concrete =
        ::jxx::NEW<AbstractSetCountingConsumer2>();

    const auto consumer =
        ::jxx::CAST<
        ::jxx::util::function::Consumer<S>>(
            concrete);

    ASSERT_NE(nullptr, consumer);

    // Confirms this test's interface object dispatches to this exact
    // concrete consumer before involving the spliterator.
    consumer->accept(::jxx::NEW<S>("direct"));
    ASSERT_EQ(1, concrete->count);

    spliterator->forEachRemaining(consumer);

    EXPECT_EQ(3, concrete->count);
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
