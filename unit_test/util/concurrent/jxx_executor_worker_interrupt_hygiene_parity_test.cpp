#include <gtest/gtest.h>
#include <atomic>

#include "lang/jxx.lang.Runnable.h"
#include "lang/jxx.lang.Thread.h"
#include "util/concurrent/jxx.util.concurrent.Executors.h"
#include "util/concurrent/jxx.util.concurrent.TimeUnit.h"

namespace {
class InterruptSelf final
    : public ::jxx::lang::ClassBase<InterruptSelf,
          ::jxx::lang::Object, ::jxx::lang::Runnable> {
public:
    void run() override {
        ::jxx::lang::Thread::currentThread()->interrupt();
    }
};

class RecordInterruptStatus final
    : public ::jxx::lang::ClassBase<RecordInterruptStatus,
          ::jxx::lang::Object, ::jxx::lang::Runnable> {
public:
    void run() override {
        interrupted_.store(
            ::jxx::lang::Thread::currentThread()->isInterrupted());
        ran_.store(true);
    }
    ::jxx::lang::jbool ran() const noexcept { return ran_.load(); }
    ::jxx::lang::jbool interrupted() const noexcept {
        return interrupted_.load();
    }
private:
    std::atomic<::jxx::lang::jbool> ran_{false};
    std::atomic<::jxx::lang::jbool> interrupted_{false};
};

TEST(ExecutorWorkerInterruptHygieneParity,
     ScheduledWorkerDoesNotLeakInterruptToNextTask) {
    using namespace ::jxx::util::concurrent;
    const auto service = Executors::newSingleThreadScheduledExecutor();
    const auto first = service->schedule(
        ::jxx::CAST<::jxx::lang::Runnable>(::jxx::NEW<InterruptSelf>()),
        0, TimeUnit::NANOSECONDS());
    (void)first->get(2, TimeUnit::SECONDS());

    const auto recorder = ::jxx::NEW<RecordInterruptStatus>();
    const auto second = service->schedule(
        ::jxx::CAST<::jxx::lang::Runnable>(recorder),
        0, TimeUnit::NANOSECONDS());
    (void)second->get(2, TimeUnit::SECONDS());

    EXPECT_TRUE(recorder->ran());
    EXPECT_FALSE(recorder->interrupted());
    service->shutdown();
}
} // namespace
