#include <gtest/gtest.h>
#include <limits>

#include "lang/jxx.lang.Runnable.h"
#include "util/concurrent/jxx.util.concurrent.Delayed.h"
#include "util/concurrent/jxx.util.concurrent.ScheduledFutureTask.h"
#include "util/concurrent/jxx.util.concurrent.TimeUnit.h"

namespace {

class NoOpRunnable final
    : public ::jxx::lang::ClassBase<
          NoOpRunnable,
          ::jxx::lang::Object,
          ::jxx::lang::Runnable> {
public:
    void run() override {}
};

class FixedDelay final
    : public ::jxx::lang::ClassBase<
          FixedDelay,
          ::jxx::lang::Object,
          ::jxx::util::concurrent::Delayed> {
public:
    explicit FixedDelay(::jxx::lang::jlong nanos) : nanos_(nanos) {}

    ::jxx::lang::jlong getDelay(
        const ::jxx::Ptr<::jxx::util::concurrent::TimeUnit>& unit) const override {
        if (unit == nullptr) throw ::jxx::lang::NullPointerException();
        return unit->convert(
            nanos_, ::jxx::util::concurrent::TimeUnit::NANOSECONDS());
    }

    ::jxx::lang::jint compareTo(
        const ::jxx::Ptr<::jxx::util::concurrent::Delayed>& other) const override {
        if (other == nullptr) throw ::jxx::lang::NullPointerException();
        const auto rhs = other->getDelay(
            ::jxx::util::concurrent::TimeUnit::NANOSECONDS());
        return nanos_ < rhs ? -1 : nanos_ > rhs ? 1 : 0;
    }

private:
    ::jxx::lang::jlong nanos_;
};

TEST(ScheduledFutureParity, CompareToDoesNotOverflowAtDelayExtremes) {
    using namespace ::jxx::util::concurrent;
    const auto task = ::jxx::NEW<ScheduledFutureTask<::jxx::lang::Object>>(
        ::jxx::CAST<::jxx::lang::Runnable>(::jxx::NEW<NoOpRunnable>()),
        nullptr,
        0);
    const auto minimum = ::jxx::CAST<Delayed>(::jxx::NEW<FixedDelay>(
        std::numeric_limits<::jxx::lang::jlong>::min()));
    const auto maximum = ::jxx::CAST<Delayed>(::jxx::NEW<FixedDelay>(
        std::numeric_limits<::jxx::lang::jlong>::max()));

    EXPECT_GT(task->compareTo(minimum), 0);
    EXPECT_LT(task->compareTo(maximum), 0);
}

TEST(ScheduledFutureParity, CompareToRejectsNull) {
    using namespace ::jxx::util::concurrent;
    const auto task = ::jxx::NEW<ScheduledFutureTask<::jxx::lang::Object>>(
        ::jxx::CAST<::jxx::lang::Runnable>(::jxx::NEW<NoOpRunnable>()),
        nullptr,
        0);
    EXPECT_ANY_THROW(task->compareTo(nullptr));
}

} // namespace
