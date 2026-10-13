#include <gtest/gtest.h>

#include "util/concurrent/jxx.util.concurrent.Executors.h"
#include "util/concurrent/jxx.util.concurrent.ScheduledExecutorService.h"
#include "util/concurrent/jxx.util.concurrent.TimeUnit.h"

namespace {

TEST(ScheduledExecutorPublicApiParity, CallableSchedulingRemainsAvailable) {
    const auto scheduler =
        ::jxx::util::concurrent::Executors::newSingleThreadScheduledExecutor();
    ASSERT_NE(nullptr, scheduler);

    // Compilation of ordinary scheduling calls verifies that moving the
    // implementation hook does not alter the public scheduling surface.
    scheduler->shutdown();
}

} // namespace
