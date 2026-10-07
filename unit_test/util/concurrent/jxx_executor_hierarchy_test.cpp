#include <gtest/gtest.h>

#include "util/concurrent/jxx.util.concurrent.CountedCompleter.h"
#include "util/concurrent/jxx.util.concurrent.ThreadPoolExecutor.h"
#include "lang/jxx.lang.String.h"

TEST(JxxExecutorHierarchyTest, PublicExecutorClassesHaveDistinctMetadata) {
    using Completer = ::jxx::util::concurrent::CountedCompleter<::jxx::lang::String>;
    using Task = ::jxx::util::concurrent::ForkJoinTask<::jxx::lang::String>;

    EXPECT_NE(Completer::Class(), Task::Class());
    EXPECT_NE(::jxx::util::concurrent::ThreadPoolExecutor::Class(),
              ::jxx::util::concurrent::AbstractExecutorService::Class());
}
