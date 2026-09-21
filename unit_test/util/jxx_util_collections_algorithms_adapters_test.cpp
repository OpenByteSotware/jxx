#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "util/jxx.util.ArrayList.h"
#include "util/jxx.util.Collections.h"

namespace {
using S = ::jxx::lang::String;
using ::jxx::util::ArrayList;
using ::jxx::util::Collections;
using ::jxx::util::Collection;
using ::jxx::util::List;

TEST(CollectionsAlgorithmsParityTest, SortSearchReverseRotateAndReplace) {
    const auto concrete = ::jxx::NEW<ArrayList<S>>();
    concrete->add(::jxx::NEW<S>("c"));
    concrete->add(::jxx::NEW<S>("a"));
    concrete->add(::jxx::NEW<S>("b"));
    const auto list = ::jxx::CAST<List<S>>(concrete);

    Collections::sort(list);
    EXPECT_TRUE(list->get(0)->utf8() == "a");
    EXPECT_EQ(1, Collections::binarySearch(list, ::jxx::NEW<S>("b")));
    Collections::reverse(list);
    EXPECT_TRUE(list->get(0)->utf8() == "c");
    Collections::rotate(list, 1);
    EXPECT_TRUE(list->get(0)->utf8() == "a");
    EXPECT_TRUE(Collections::replaceAll(
        list, ::jxx::NEW<S>("b"), ::jxx::NEW<S>("B")));
}

TEST(CollectionsAlgorithmsParityTest, FillCopyFrequencyAndDisjoint) {
    const auto destinationConcrete = ::jxx::NEW<ArrayList<S>>();
    destinationConcrete->add(::jxx::NEW<S>("x"));
    destinationConcrete->add(::jxx::NEW<S>("x"));
    const auto sourceConcrete = ::jxx::NEW<ArrayList<S>>();
    sourceConcrete->add(::jxx::NEW<S>("a"));
    sourceConcrete->add(::jxx::NEW<S>("a"));
    const auto destination = ::jxx::CAST<List<S>>(destinationConcrete);
    const auto source = ::jxx::CAST<List<S>>(sourceConcrete);

    Collections::copy(destination, source);
    EXPECT_TRUE(destination->get(0)->utf8() == "a");
    Collections::fill(destination, ::jxx::NEW<S>("z"));
    EXPECT_TRUE(destination->get(1)->utf8() == "z");
}

TEST(CollectionsAdaptersParityTest, EnumerationAndListRoundTripInOrder) {
    const auto concrete = ::jxx::NEW<ArrayList<S>>();
    concrete->add(::jxx::NEW<S>("a"));
    concrete->add(::jxx::NEW<S>("b"));
    const auto collection = ::jxx::CAST<Collection<S>>(concrete);

    const auto enumeration = Collections::enumeration(collection);
    const auto result = Collections::list(enumeration);
    ASSERT_EQ(2, result->size());
    EXPECT_TRUE(result->get(0)->utf8() == "a");
    EXPECT_TRUE(result->get(1)->utf8() == "b");
}

} // namespace
