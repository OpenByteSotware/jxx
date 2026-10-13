#include <gtest/gtest.h>
#include <atomic>

#include "lang/jxx.lang.Runnable.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.Thread.h"
#include "util/concurrent/jxx.util.concurrent.Executors.h"
#include "util/concurrent/jxx.util.concurrent.ThreadFactory.h"
#include "util/concurrent/jxx.util.concurrent.TimeUnit.h"

namespace {
class RecordingFactory final
    : public ::jxx::lang::ClassBase<RecordingFactory,
          ::jxx::lang::Object,
          ::jxx::util::concurrent::ThreadFactory> {
public:
    ::jxx::Ptr<::jxx::lang::Thread> newThread(
        const ::jxx::Ptr<::jxx::lang::Runnable>& runnable) override {
        called_.fetch_add(1);
        return ::jxx::NEW<::jxx::lang::Thread>(
            runnable,
            ::jxx::NEW<::jxx::lang::String>("recording-factory-thread"));
    }
    ::jxx::lang::jint called() const noexcept { return called_.load(); }
private:
    std::atomic<::jxx::lang::jint> called_{0};
};

class CaptureThreadName final
    : public ::jxx::lang::ClassBase<CaptureThreadName,
          ::jxx::lang::Object, ::jxx::lang::Runnable> {
public:
    void run() override {
        name_ = ::jxx::lang::Thread::currentThread()->getName();
        ran_.store(true);
    }
    ::jxx::lang::jbool ran() const noexcept { return ran_.load(); }
    ::jxx::Ptr<::jxx::lang::String> name() const { return name_; }
private:
    std::atomic<::jxx::lang::jbool> ran_{false};
    ::jxx::Ptr<::jxx::lang::String> name_;
};

TEST(ExecutorThreadFactoryParity, ScheduledPoolUsesSuppliedThreadFactory) {
    using namespace ::jxx::util::concurrent;
    const auto factory = ::jxx::NEW<RecordingFactory>();
    const auto service = Executors::newScheduledThreadPool(
        1, ::jxx::CAST<ThreadFactory>(factory));
    const auto command = ::jxx::NEW<CaptureThreadName>();
    const auto future = service->schedule(
        ::jxx::CAST<::jxx::lang::Runnable>(command),
        0, TimeUnit::NANOSECONDS());
    (void)future->get(2, TimeUnit::SECONDS());
    EXPECT_TRUE(command->ran());
    ASSERT_NE(nullptr, command->name());
    EXPECT_EQ("recording-factory-thread", command->name()->utf8());
    EXPECT_GE(factory->called(), 1);
    service->shutdown();
}

TEST(ExecutorThreadFactoryParity, FixedPoolUsesSuppliedThreadFactory) {
    using namespace ::jxx::util::concurrent;
    const auto factory = ::jxx::NEW<RecordingFactory>();
    const auto service = Executors::newFixedThreadPool(
        1, ::jxx::CAST<ThreadFactory>(factory));
    const auto command = ::jxx::NEW<CaptureThreadName>();
    service->execute(::jxx::CAST<::jxx::lang::Runnable>(command));
    for (::jxx::lang::jint i = 0; i < 200 && !command->ran(); ++i) {
        ::jxx::lang::Thread::sleep(1);
    }
    EXPECT_TRUE(command->ran());
    EXPECT_GE(factory->called(), 1);
    service->shutdown();
}
} // namespace
