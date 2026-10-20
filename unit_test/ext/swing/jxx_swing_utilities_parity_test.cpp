#include <gtest/gtest.h>

#include <atomic>
#include <condition_variable>
#include <mutex>

#include "jxx.initialize.h"
#include "ext/swing/jxx.ext.swing.SwingUtilities.h"
#include "lang/jxx.lang.InvocationTargetException.h"
#include "lang/jxx.lang.Runnable.h"
#include "lang/jxx.lang.RuntimeException.h"

namespace {

class FunctionRunnable final
    : public ::jxx::lang::ClassBase<
          FunctionRunnable, ::jxx::lang::Object, ::jxx::lang::Runnable> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<
        FunctionRunnable, JxxSuper, ::jxx::lang::Runnable>;

    explicit FunctionRunnable(std::function<void()> function)
        : Super(), function_(std::move(function)) {}
    void run() override { function_(); }
private:
    std::function<void()> function_;
};

TEST(SwingUtilitiesParity, InvokeAndWaitRunsOnDispatchThread)
{
    ::jxx::initialize();
    std::atomic<bool> ran{false};
    std::atomic<bool> onDispatchThread{false};
    auto runnable = ::jxx::NEW<FunctionRunnable>([&] {
        ran.store(true);
        onDispatchThread.store(
            ::jxx::ext::swing::SwingUtilities::isEventDispatchThread());
    });
    ::jxx::ext::swing::SwingUtilities::invokeAndWait(runnable);
    EXPECT_TRUE(ran.load());
    EXPECT_TRUE(onDispatchThread.load());
}

TEST(SwingUtilitiesParity, InvokeLaterPrecedesFollowingInvokeAndWait)
{
    ::jxx::initialize();
    std::atomic<int> state{0};
    ::jxx::ext::swing::SwingUtilities::invokeLater(
        ::jxx::NEW<FunctionRunnable>([&] { state.store(1); }));
    ::jxx::ext::swing::SwingUtilities::invokeAndWait(
        ::jxx::NEW<FunctionRunnable>([&] { EXPECT_EQ(state.load(), 1); }));
}

TEST(SwingUtilitiesParity, InvokeAndWaitWrapsTaskFailure)
{
    ::jxx::initialize();
    auto runnable = ::jxx::NEW<FunctionRunnable>([] {
        throw ::jxx::lang::RuntimeException("failure");
    });
    try {
        ::jxx::ext::swing::SwingUtilities::invokeAndWait(runnable);
        FAIL() << "Expected InvocationTargetException";
    }
    catch (const ::jxx::lang::InvocationTargetException& exception) {
        EXPECT_NE(exception.getTargetException(), nullptr);
    }
}

} // namespace
