#include <gtest/gtest.h>
#include <string>

#include "lang/jxx.lang.Runnable.h"
#include "lang/jxx.lang.Thread.h"
#include "util/concurrent/jxx.util.concurrent.Executors.h"

namespace {

class NoOpRunnable final
    : public ::jxx::lang::ClassBase<
          NoOpRunnable,
          ::jxx::lang::Object,
          ::jxx::lang::Runnable> {
public:
    void run() override {}
};

TEST(ExecutorsJava8Parity, DefaultFactoriesUseDistinctPoolAndThreadSequences) {
    using ::jxx::util::concurrent::Executors;
    const auto runnable = ::jxx::CAST<::jxx::lang::Runnable>(
        ::jxx::NEW<NoOpRunnable>());
    const auto firstFactory = Executors::defaultThreadFactory();
    const auto secondFactory = Executors::defaultThreadFactory();
    ASSERT_NE(nullptr, firstFactory);
    ASSERT_NE(nullptr, secondFactory);
    EXPECT_NE(firstFactory, secondFactory);

    const auto first = firstFactory->newThread(runnable);
    const auto second = firstFactory->newThread(runnable);
    const auto third = secondFactory->newThread(runnable);
    ASSERT_NE(nullptr, first);
    ASSERT_NE(nullptr, second);
    ASSERT_NE(nullptr, third);

    const auto firstName = first->getName()->utf8();
    const auto secondName = second->getName()->utf8();
    const auto thirdName = third->getName()->utf8();
    EXPECT_NE(std::string::npos, firstName.find("-thread-1"));
    EXPECT_NE(std::string::npos, secondName.find("-thread-2"));
    EXPECT_NE(std::string::npos, thirdName.find("-thread-1"));
    EXPECT_NE(firstName.substr(0, firstName.find("-thread-")),
              thirdName.substr(0, thirdName.find("-thread-")));

    EXPECT_FALSE(first->isDaemon());
    EXPECT_EQ(::jxx::lang::Thread::NORM_PRIORITY, first->getPriority());
}

TEST(ExecutorsJava8Parity, DefaultFactoryRejectsNullRunnable) {
    const auto factory =
        ::jxx::util::concurrent::Executors::defaultThreadFactory();
    EXPECT_ANY_THROW(factory->newThread(nullptr));
}

} // namespace
