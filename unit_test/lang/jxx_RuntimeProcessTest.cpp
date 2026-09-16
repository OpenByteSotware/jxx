#include <gtest/gtest.h>

#include "lang/jxx.lang.Process.h"
#include "lang/jxx.lang.Runtime.h"
#include "lang/jxx.lang.String.h"
#include "ProcessTestSupport.h"

namespace {
using jxx::test::process::commandArray;
using jxx::test::process::readAll;
using jxx::test::process::shellCommand;

TEST(RuntimeProcessTest, RuntimeIsSingleton) {
    EXPECT_EQ(
        jxx::lang::Runtime::getRuntime().get(),
        jxx::lang::Runtime::getRuntime().get());
}

TEST(RuntimeProcessTest, ExecutesCommandArray) {
    auto process = jxx::lang::Runtime::getRuntime()->exec(
        commandArray(shellCommand("printf runtime_array")));
    EXPECT_EQ(process->waitFor(), 0);
    EXPECT_EQ(readAll(process->getInputStream()), "runtime_array");
}

TEST(RuntimeProcessTest, ExecutesStringCommand) {
#ifdef _WIN32
    auto command = jxx::NEW<jxx::lang::String>("cmd.exe /d /s /c exit 0");
#else
    auto command = jxx::NEW<jxx::lang::String>("/bin/sh -c true");
#endif
    EXPECT_EQ(jxx::lang::Runtime::getRuntime()->exec(command)->waitFor(), 0);
}

TEST(RuntimeProcessTest, EnvironmentArrayReplacesChildEnvironment) {
#ifdef _WIN32
    auto command = commandArray(shellCommand("if \"%JXX_RUNTIME_TEST%\"==\"value42\" (exit 0) else (exit 9)"));
#else
    auto command = commandArray(shellCommand("test \"$JXX_RUNTIME_TEST\" = value42"));
#endif
    auto environment = commandArray({"JXX_RUNTIME_TEST=value42"});
    EXPECT_EQ(jxx::lang::Runtime::getRuntime()->exec(command, environment)->waitFor(), 0);
}

} // namespace
