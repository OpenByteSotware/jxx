#include <gtest/gtest.h>

#include <stdexcept>
#include <utility>
#include <vector>

#include "lang/jxx.lang.Finally.h"

TEST(FinallyTest, RunsAtNormalScopeExit)
{
    bool ran = false;
    {
        auto guard = ::jxx::lang::makeFinally([&]() { ran = true; });
        (void)guard;
    }
    EXPECT_TRUE(ran);
}

TEST(FinallyTest, RunsBeforePendingExceptionIsRethrown)
{
    std::vector<int> order;
    EXPECT_THROW(
        {
            try {
                auto guard = ::jxx::lang::makeFinally([&]() {
                    order.push_back(2);
                });
                try {
                    order.push_back(1);
                    throw std::runtime_error("body");
                }
                catch (const std::logic_error&) {
                    FAIL() << "wrong handler";
                }
            }
            catch (...) {
                // This mirrors converter output around source try/catch.
                auto guard = ::jxx::lang::makeFinally([&]() {});
                guard.dismiss();
                throw;
            }
        },
        std::runtime_error);
    // A direct scope guard also executes during stack unwinding.
    ASSERT_EQ(order.size(), 2U);
    EXPECT_EQ(order[0], 1);
    EXPECT_EQ(order[1], 2);
}

TEST(FinallyTest, RunAndDismissExecutesExactlyOnce)
{
    int calls = 0;
    {
        auto guard = ::jxx::lang::makeFinally([&]() { ++calls; });
        guard.runAndDismiss();
    }
    EXPECT_EQ(calls, 1);
}

TEST(FinallyTest, DismissPreventsExecution)
{
    int calls = 0;
    {
        auto guard = ::jxx::lang::makeFinally([&]() { ++calls; });
        guard.dismiss();
    }
    EXPECT_EQ(calls, 0);
}

TEST(FinallyTest, MovingTransfersResponsibility)
{
    int calls = 0;
    {
        auto first = ::jxx::lang::makeFinally([&]() { ++calls; });
        auto second = std::move(first);
        (void)second;
    }
    EXPECT_EQ(calls, 1);
}

TEST(FinallyTest, CallbackExceptionPropagatesOnNormalExit)
{
    EXPECT_THROW(
        {
            auto guard = ::jxx::lang::makeFinally([]() {
                throw std::runtime_error("finally");
            });
            (void)guard;
        },
        std::runtime_error);
}
