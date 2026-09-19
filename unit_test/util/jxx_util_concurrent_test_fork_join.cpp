#include <gtest/gtest.h>
#include <atomic>

#include "lang/jxx.lang.Integer.h"
#include "util/jxx.util.concurrent.CountedCompleter.h"
#include "util/jxx.util.concurrent.ForkJoinPool.h"
#include "util/jxx.util.concurrent.RecursiveAction.h"
#include "util/jxx.util.concurrent.RecursiveTask.h"

namespace {
class SumTask final : public jxx::util::concurrent::RecursiveTask<jxx::lang::Integer> {
public:
    SumTask(int begin, int end) : begin_(begin), end_(end) {}
protected:
    jxx::Ptr<jxx::lang::Integer> compute() override {
        if (end_ - begin_ <= 8) {
            int sum = 0;
            for (int i = begin_; i < end_; ++i) sum += i;
            return jxx::lang::Integer::valueOf(sum);
        }
        const int middle = begin_ + (end_ - begin_) / 2;
        auto left = jxx::NEW<SumTask>(begin_, middle);
        auto right = jxx::NEW<SumTask>(middle, end_);
        left->fork();
        auto rightResult = right->invoke();
        auto leftResult = left->join();
        return jxx::lang::Integer::valueOf(
            leftResult->intValue() + rightResult->intValue());
    }
private:
    int begin_;
    int end_;
};

class IncrementAction final : public jxx::util::concurrent::RecursiveAction {
public:
    explicit IncrementAction(std::atomic<int>& value) : value_(value) {}
protected:
    void compute() override { value_.fetch_add(1); }
private:
    std::atomic<int>& value_;
};

TEST(ForkJoinTaskTest, RecursiveTaskReturnsJoinedResult) {
    auto task = jxx::NEW<SumTask>(1, 101);
    EXPECT_EQ(task->invoke()->intValue(), 5050);
    EXPECT_TRUE(task->isDone());
}

TEST(ForkJoinTaskTest, RecursiveActionCompletesWithoutResult) {
    std::atomic<int> value{0};
    auto action = jxx::NEW<IncrementAction>(value);
    EXPECT_EQ(action->invoke(), nullptr);
    EXPECT_EQ(value.load(), 1);
}

TEST(ForkJoinPoolTest, ThresholdSelectsParallelRangeExecution) {
    auto pool = jxx::util::concurrent::ForkJoinPool::commonPool();
    std::atomic<int> visits{0};
    pool->parallelFor(256, 1, [&](std::size_t begin, std::size_t end) {
        visits.fetch_add(static_cast<int>(end - begin));
    });
    EXPECT_EQ(visits.load(), 256);
}
}
