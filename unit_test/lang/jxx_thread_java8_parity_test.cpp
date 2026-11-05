#include <gtest/gtest.h>
#include "lang/jxx.lang.Thread.h"

TEST(ThreadJava8Parity, EnumerationAndContextLoaderMethodsAreAvailable) {
    auto current = ::jxx::lang::Thread::currentThread();
    ASSERT_NE(nullptr, current);
    EXPECT_GE(::jxx::lang::Thread::activeCount(), 1);
    auto threads = ::jxx::NEW<::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::Thread>, 1U>>(8);
    EXPECT_GE(::jxx::lang::Thread::enumerate(threads), 1);
    current->setContextClassLoader(nullptr);
    EXPECT_EQ(nullptr, current->getContextClassLoader());
    EXPECT_NE(nullptr, current->getStackTrace());
}
