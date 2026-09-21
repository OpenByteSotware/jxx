#include <gtest/gtest.h>

#include "lang/jxx.lang.Runtime.h"
#include "lang/jxx.lang.System.h"

namespace {
using ::jxx::lang::Runtime;
using ::jxx::lang::System;

TEST(RuntimeFinalizationParityTest, DeprecatedFinalizationToggleIsCallable) {
    Runtime::runFinalizersOnExit(false);
    Runtime::runFinalizersOnExit(true);
    Runtime::runFinalizersOnExit(false);
}

TEST(SystemFinalizationParityTest, DeprecatedFinalizationToggleDelegates) {
    System::runFinalizersOnExit(false);
    System::runFinalizersOnExit(true);
    System::runFinalizersOnExit(false);
}

TEST(RuntimeFinalizationParityTest, ExplicitFinalizationEntryPointsRemainCallable) {
    Runtime::getRuntime()->runFinalization();
    System::runFinalization();
}

} // namespace
