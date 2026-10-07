#include <gtest/gtest.h>
#include "util/jxx.util.AbstractSet.h"
#include "util/jxx.util.AbstractSequentialList.h"
#include "util/concurrent/jxx.util.concurrent.RecursiveAction.h"
#include "util/concurrent/jxx.util.concurrent.RecursiveTask.h"
#include "lang/jxx.lang.String.h"
TEST(JxxCoreAbstractHierarchyTest, AbstractClassesHaveDistinctMetadata) {
    using SetType = ::jxx::util::AbstractSet<::jxx::lang::String>;
    using CollectionType = ::jxx::util::AbstractCollection<::jxx::lang::String>;
    using SequentialType = ::jxx::util::AbstractSequentialList<::jxx::lang::String>;
    using ListType = ::jxx::util::AbstractList<::jxx::lang::String>;
    using TaskType = ::jxx::util::concurrent::RecursiveTask<::jxx::lang::String>;
    using ForkTaskType = ::jxx::util::concurrent::ForkJoinTask<::jxx::lang::String>;
    EXPECT_NE(SetType::Class(), CollectionType::Class());
    EXPECT_NE(SequentialType::Class(), ListType::Class());
    EXPECT_NE(::jxx::util::concurrent::RecursiveAction::Class(), ::jxx::util::concurrent::ForkJoinTask<::jxx::lang::Object>::Class());
    EXPECT_NE(TaskType::Class(), ForkTaskType::Class());
}
