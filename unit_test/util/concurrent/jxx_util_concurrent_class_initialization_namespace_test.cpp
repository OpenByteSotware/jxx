#include <gtest/gtest.h>

#include "util/concurrent/jxx.util.concurrent.CancellableTask.h"
#include "util/concurrent/jxx.util.concurrent.ClassInitialization.h"
#include "util/concurrent/jxx.util.concurrent.ScheduledFutureAnyAdapter.h"
#include "util/concurrent/jxx.util.concurrent.ScheduledTask.h"

TEST(JxxUtilConcurrentClassInitializationNamespace, InternalMetadataTypesAreAvailable)
{
    ::jxx::util::concurrent::initializeClasses();
    EXPECT_NE(::jxx::util::concurrent::internal::CancellableTask::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(::jxx::util::concurrent::scheduled_detail::ScheduledFutureAnyAdapter::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(::jxx::util::concurrent::scheduled_detail::ScheduledTask::JxxClassInfoMarker::Class(), nullptr);
}
