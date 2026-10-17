#include <gtest/gtest.h>
#include <type_traits>
#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "jxx.initialize.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.TreeSet.h"

using StringTreeSet=::jxx::util::TreeSet<::jxx::lang::String>;
static_assert(!std::is_abstract_v<StringTreeSet>);
static_assert(std::is_default_constructible_v<StringTreeSet>);

TEST(TreeSetParity, ImplementsSortedAndNavigableSetContracts) {
    auto set=::jxx::NEW<StringTreeSet>();
    set->add(::jxx::NEW<::jxx::lang::String>("b"));
    set->add(::jxx::NEW<::jxx::lang::String>("a"));
    set->add(::jxx::NEW<::jxx::lang::String>("c"));
    EXPECT_EQ(set->first()->utf8(),"a");
    EXPECT_EQ(set->last()->utf8(),"c");
    EXPECT_EQ(set->lower(::jxx::NEW<::jxx::lang::String>("b"))->utf8(),"a");
    auto descending=set->descendingIterator();
    EXPECT_EQ(descending->next()->utf8(),"c");
}

TEST(TreeSetSerialization, RoundTripsElementsInSortedOrder) {
    ::jxx::initialize();
    auto source=::jxx::NEW<StringTreeSet>();
    source->add(::jxx::NEW<::jxx::lang::String>("b"));
    source->add(::jxx::NEW<::jxx::lang::String>("a"));
    auto bytes=::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    auto output=::jxx::NEW<::jxx::io::ObjectOutputStream>(bytes);
    output->writeObject(source); 
    output->flush();
    auto input=::jxx::NEW<::jxx::io::ObjectInputStream>(
        ::jxx::NEW<::jxx::io::ByteArrayInputStream>(bytes->toByteArray()));
    auto restored=std::dynamic_pointer_cast<StringTreeSet>(input->readObject());
    ASSERT_NE(restored,nullptr); ASSERT_EQ(restored->size(),2);
    EXPECT_EQ(restored->first()->utf8(),"a");
    EXPECT_EQ(restored->last()->utf8(),"b");
}
