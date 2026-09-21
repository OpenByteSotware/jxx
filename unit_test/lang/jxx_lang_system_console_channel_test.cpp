#include <gtest/gtest.h>

#include "io/jxx.io.Console.h"
#include "lang/jxx.lang.System.h"
#include "nio/channels/jxx.nio.channels.Channel.h"

namespace {
using ::jxx::lang::System;

TEST(SystemConsoleParityTest, ConsoleResultIsStableForRuntime) {
    const auto first = System::console();
    const auto second = System::console();
    EXPECT_EQ(first.get(), second.get());
    if (first != nullptr) {
        EXPECT_NE(nullptr, first->reader());
        EXPECT_NE(nullptr, first->writer());
    }
}

TEST(SystemInheritedChannelParityTest, NoLauncherChannelReturnsNull) {
    EXPECT_EQ(nullptr, System::inheritedChannel());
}

} // namespace
