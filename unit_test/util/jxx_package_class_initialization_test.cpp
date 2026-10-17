#include <gtest/gtest.h>
#include <type_traits>

#include "jxx.initialize.h"
#include "lang/jxx.lang.Object.h"
#include "util/jxx.util.ClassInitialization.h"
#include "util/jxx.util.TreeSet.h"

TEST(PackageClassInitialization, UtilHierarchyIsIdempotent) {
    EXPECT_NO_THROW(::jxx::util::initializeClasses());
    EXPECT_NO_THROW(::jxx::util::initializeClasses());
}

TEST(PackageClassInitialization, GlobalInitializationRemainsIdempotent) {
    EXPECT_NO_THROW(::jxx::initialize());
    EXPECT_NO_THROW(::jxx::initialize());
}

TEST(PackageClassInitialization, ErasedTreeSetIsConcrete) {
    using ErasedTreeSet = ::jxx::util::TreeSet<::jxx::lang::Object>;
    static_assert(!std::is_abstract_v<ErasedTreeSet>);
    static_assert(std::is_default_constructible_v<ErasedTreeSet>);
    const auto value = ::jxx::NEW<ErasedTreeSet>();
    EXPECT_NE(value, nullptr);
}
