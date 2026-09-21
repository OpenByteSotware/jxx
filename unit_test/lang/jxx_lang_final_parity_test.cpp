#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.Thread.h"
#include "lang/jxx.lang.UnsupportedOperationException.h"
#include "util/jxx.util.LinkedHashMap.h"

namespace {

TEST(StringInternParityTest, EqualValuesReturnOneCanonicalReference) {
    const auto first = ::jxx::NEW<::jxx::lang::String>("canonical");
    const auto second = ::jxx::NEW<::jxx::lang::String>("canonical");
    const auto firstInterned = first->intern();
    const auto secondInterned = second->intern();
    ASSERT_NE(nullptr, firstInterned);
    EXPECT_EQ(firstInterned.get(), secondInterned.get());
    EXPECT_EQ(firstInterned.get(), first->intern().get());
}

TEST(StringInternParityTest, DistinctValuesRemainDistinctReferences) {
    const auto first = ::jxx::NEW<::jxx::lang::String>("first")->intern();
    const auto second = ::jxx::NEW<::jxx::lang::String>("second")->intern();
    EXPECT_NE(first.get(), second.get());
}

TEST(ThreadCompatibilityBoundaryTest, UnsafeControlsReportUnsupportedOperation) {
    const auto thread = ::jxx::NEW<::jxx::lang::Thread>();
    EXPECT_THROW(thread->stop(), ::jxx::lang::UnsupportedOperationException);
    EXPECT_THROW(thread->suspend(), ::jxx::lang::UnsupportedOperationException);
    EXPECT_THROW(thread->resume(), ::jxx::lang::UnsupportedOperationException);
    EXPECT_THROW(thread->destroy(), ::jxx::lang::UnsupportedOperationException);
}

TEST(ThreadCompatibilityBoundaryTest, InactiveThreadHasNoStackFrames) {
    const auto thread = ::jxx::NEW<::jxx::lang::Thread>();
    EXPECT_EQ(0, thread->countStackFrames());
}

TEST(ClassStructureParityTest, UpdatedAliasInstantiatesNormally) {
    using MapType = ::jxx::util::LinkedHashMap<
        ::jxx::lang::String,
        ::jxx::lang::String>;
    const auto map = ::jxx::NEW<MapType>();
    EXPECT_TRUE(map->isEmpty());
}

} // namespace
