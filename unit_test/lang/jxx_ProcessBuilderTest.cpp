#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <string>

#include "io/jxx.io.File.h"
#include "io/jxx.io.IOException.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IllegalThreadStateException.h"
#include "lang/jxx.lang.Process.h"
#include "lang/jxx.lang.ProcessBuilder.h"
#include "lang/jxx.lang.String.h"
#include "ProcessTestSupport.h"
#include "util/jxx.util.concurrent.TimeUnit.h"

namespace {
using jxx::test::process::builder;
using jxx::test::process::commandArray;
using jxx::test::process::readAll;
using jxx::test::process::shellCommand;
using jxx::test::process::sleepCommand;
using jxx::test::process::temporaryPath;

TEST(ProcessBuilderTest, CommandRoundTripPreservesArguments) {
    auto value = builder({"program", "argument with spaces", ""});
    auto command = value->command();
    ASSERT_EQ(command->length, 3u);
    EXPECT_EQ((*command)[0]->utf8(), "program");
    EXPECT_EQ((*command)[1]->utf8(), "argument with spaces");
    EXPECT_EQ((*command)[2]->utf8(), "");
}

TEST(ProcessBuilderTest, EmptyCommandIsRejected) {
    EXPECT_THROW(
        jxx::NEW<jxx::lang::ProcessBuilder>(commandArray({})),
        jxx::lang::IllegalArgumentException);
}

TEST(ProcessBuilderTest, DirectoryRoundTrip) {
    auto value = builder(shellCommand("exit 0"));
    auto directory = jxx::NEW<jxx::io::File>(
        jxx::NEW<jxx::lang::String>(std::filesystem::current_path().u8string()));
    EXPECT_EQ(value->directory(directory).get(), value.get());
    EXPECT_EQ(value->directory().get(), directory.get());
}

TEST(ProcessBuilderTest, EnvironmentRoundTripAndClear) {
    auto value = builder(shellCommand("exit 0"));
    auto name = jxx::NEW<jxx::lang::String>("JXX_PROCESS_TEST_VALUE");
    auto data = jxx::NEW<jxx::lang::String>("expected-value");
    EXPECT_EQ(value->environment(name, data).get(), value.get());
    ASSERT_NE(value->environment(name), nullptr);
    EXPECT_EQ(value->environment(name)->utf8(), "expected-value");
    value->clearEnvironment();
    EXPECT_EQ(value->environment(name), nullptr);
}

TEST(ProcessBuilderTest, StartsAndCapturesStandardOutput) {
    auto process = builder(shellCommand("printf jxx_process_output"))->start();
    EXPECT_EQ(process->waitFor(), 0);
    EXPECT_EQ(readAll(process->getInputStream()), "jxx_process_output");
}

TEST(ProcessBuilderTest, CapturesSeparateStandardError) {
#ifdef _WIN32
    auto command = shellCommand("echo jxx_process_error 1>&2");
#else
    auto command = shellCommand("printf jxx_process_error 1>&2");
#endif
    auto process = builder(command)->start();
    EXPECT_EQ(process->waitFor(), 0);
    EXPECT_EQ(readAll(process->getErrorStream()), "jxx_process_error");
}

TEST(ProcessBuilderTest, RedirectErrorStreamMergesErrorIntoOutput) {
#ifdef _WIN32
    auto command = shellCommand("echo out & echo err 1>&2");
#else
    auto command = shellCommand("printf out; printf err 1>&2");
#endif
    auto value = builder(command);
    value->redirectErrorStream(true);
    auto process = value->start();
    EXPECT_EQ(process->waitFor(), 0);
    const auto output = readAll(process->getInputStream());
    EXPECT_NE(output.find("out"), std::string::npos);
    EXPECT_NE(output.find("err"), std::string::npos);
    EXPECT_EQ(process->getErrorStream()->read(), -1);
}

TEST(ProcessBuilderTest, RedirectOutputToFileUsesNullInputStream) {
    const auto path = temporaryPath(".txt");
    auto file = jxx::NEW<jxx::io::File>(
        jxx::NEW<jxx::lang::String>(path.u8string()));
    auto value = builder(shellCommand("printf redirected"));
    value->redirectOutput(file);
    auto process = value->start();
    EXPECT_EQ(process->waitFor(), 0);
    EXPECT_EQ(process->getInputStream()->read(), -1);
    std::ifstream input(path, std::ios::binary);
    std::string contents((std::istreambuf_iterator<char>(input)), {});
    EXPECT_EQ(contents, "redirected");
    std::error_code error;
    std::filesystem::remove(path, error);
}

TEST(ProcessBuilderTest, AppendRedirectPreservesExistingData) {
    const auto path = temporaryPath(".txt");
    { std::ofstream output(path, std::ios::binary); output << "first"; }
    auto file = jxx::NEW<jxx::io::File>(
        jxx::NEW<jxx::lang::String>(path.u8string()));
    auto value = builder(shellCommand("printf second"));
    value->redirectOutput(jxx::lang::ProcessBuilder::Redirect::appendTo(file));
    EXPECT_EQ(value->start()->waitFor(), 0);
    std::ifstream input(path, std::ios::binary);
    std::string contents((std::istreambuf_iterator<char>(input)), {});
    EXPECT_EQ(contents, "firstsecond");
    std::error_code error;
    std::filesystem::remove(path, error);
}

TEST(ProcessBuilderTest, MissingExecutableThrowsIOException) {
    EXPECT_THROW(
        builder({"jxx_process_command_that_does_not_exist_78421"})->start(),
        jxx::io::IOException);
}

TEST(ProcessTest, ExitValueThrowsWhileAlive) {
    auto process = builder(sleepCommand())->start();
    EXPECT_TRUE(process->isAlive());
    EXPECT_THROW(process->exitValue(), jxx::lang::IllegalThreadStateException);
    process->destroyForcibly();
    EXPECT_TRUE(process->waitFor(10, jxx::util::concurrent::TimeUnit::SECONDS()));
    EXPECT_FALSE(process->isAlive());
}

TEST(ProcessTest, TimedWaitTimesOutThenCompletes) {
    auto process = builder(sleepCommand())->start();
    EXPECT_FALSE(process->waitFor(10, jxx::util::concurrent::TimeUnit::MILLISECONDS()));
    process->destroyForcibly();
    EXPECT_TRUE(process->waitFor(10, jxx::util::concurrent::TimeUnit::SECONDS()));
}

TEST(ProcessTest, ConcurrentWaitersObserveSameExitCode) {
    auto process = builder(shellCommand("exit 7"))->start();
    int first = -1;
    int second = -1;
    std::thread a([&] { first = process->waitFor(); });
    std::thread b([&] { second = process->waitFor(); });
    a.join();
    b.join();
    EXPECT_EQ(first, 7);
    EXPECT_EQ(second, 7);
    EXPECT_EQ(process->exitValue(), 7);
}

} // namespace
