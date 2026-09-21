#include <gtest/gtest.h>

#include <atomic>

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IllegalThreadStateException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.Process.h"
#include "lang/jxx.lang.ProcessBuilder.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"
#include "util/jxx.util.concurrent.TimeUnit.h"

namespace {
using ::jxx::lang::IllegalArgumentException;
using ::jxx::lang::IllegalThreadStateException;
using ::jxx::lang::NullPointerException;
using ::jxx::lang::Process;
using ::jxx::lang::ProcessBuilder;
using ::jxx::lang::String;
using ::jxx::util::concurrent::TimeUnit;

class FakeProcess final : public Process
{
public:
    using Process::waitFor;

    std::atomic<bool> alive{ true };
    std::atomic<bool> destroyed{ false };

    
    ::jxx::Ptr<::jxx::io::OutputStream> getOutputStream() override { return nullptr; }
    ::jxx::Ptr<::jxx::io::InputStream> getInputStream() override { return nullptr; }
    ::jxx::Ptr<::jxx::io::InputStream> getErrorStream() override { return nullptr; }
    ::jxx::lang::jint waitFor() override { alive = false; return 0; }
    ::jxx::lang::jint exitValue() override {
        if (alive) throw IllegalThreadStateException();
        return 0;
    }
    void destroy() override { destroyed = true; alive = false; }
};

::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<String>, 1U>> command(
    std::initializer_list<const char*> values) {
    auto result = ::jxx::NEW<::jxx::lang::JxxArray<::jxx::Ptr<String>, 1U>>(
        static_cast<std::uint32_t>(values.size()));
    ::jxx::lang::jint index = 0;
    for (const auto* value : values) (*result)[index++] = ::jxx::NEW<String>(value);
    return result;
}

TEST(ProcessEdgeTest, TimedWaitValidatesUnitAndHandlesNonPositiveTimeout) {
    const auto process = ::jxx::NEW<FakeProcess>();
    EXPECT_THROW(process->waitFor(1, nullptr), ::jxx::lang::NullPointerException);
    EXPECT_FALSE(process->waitFor(0, TimeUnit::MILLISECONDS()));
    EXPECT_FALSE(process->waitFor(-1, TimeUnit::MILLISECONDS()));
    process->alive = false;
    EXPECT_TRUE(process->waitFor(0, TimeUnit::MILLISECONDS()));
}

TEST(ProcessEdgeTest, DefaultLivenessAndForcefulDestroyUseCoreHooks) {
    const auto process = ::jxx::NEW<FakeProcess>();
    EXPECT_TRUE(process->isAlive());
    EXPECT_EQ(process.get(), process->destroyForcibly().get());
    EXPECT_TRUE(process->destroyed.load());
    EXPECT_FALSE(process->isAlive());
}

TEST(ProcessBuilderEdgeTest, ConstructorCopiesCommandAndGetterReturnsCopy) {
    const auto original = command({"tool", "one"});
    const auto builder = ::jxx::NEW<ProcessBuilder>(original);
    (*original)[1] = ::jxx::NEW<String>("changed");
    auto first = builder->command();
    EXPECT_EQ("one", (*first)[1]->utf8());
    (*first)[1] = ::jxx::NEW<String>("mutated");
    EXPECT_EQ("one", (*builder->command())[1]->utf8());
}

TEST(ProcessBuilderEdgeTest, RejectsEmptyNullAndInvalidRedirectConfiguration) {
    EXPECT_THROW(::jxx::NEW<ProcessBuilder>(nullptr), IllegalArgumentException);
    const auto empty = ::jxx::NEW<::jxx::lang::JxxArray<::jxx::Ptr<String>, 1U>>(0U);
    EXPECT_THROW(::jxx::NEW<ProcessBuilder>(empty), IllegalArgumentException);

    auto withNull = command({"tool", "argument"});
    (*withNull)[1] = nullptr;
    EXPECT_THROW(::jxx::NEW<ProcessBuilder>(withNull), NullPointerException);

    const auto builder =
        ::jxx::NEW<ProcessBuilder>(command({ "tool" }));

    EXPECT_THROW(
        builder->redirectInput(
            ::jxx::Ptr<ProcessBuilder::Redirect>{}),
        NullPointerException);

    EXPECT_THROW(
        builder->redirectOutput(
            ProcessBuilder::Redirect::from(
                ::jxx::Ptr<::jxx::io::File>{})),
        NullPointerException);
}

TEST(ProcessBuilderEdgeTest, InheritIoAndErrorStreamSettingsAreRetained) {
    const auto builder = ::jxx::NEW<ProcessBuilder>(command({"tool"}));
    builder->inheritIO()->redirectErrorStream(true);
    EXPECT_EQ(ProcessBuilder::Redirect::Type::INHERIT, builder->redirectInput()->type());
    EXPECT_EQ(ProcessBuilder::Redirect::Type::INHERIT, builder->redirectOutput()->type());
    EXPECT_EQ(ProcessBuilder::Redirect::Type::INHERIT, builder->redirectError()->type());
    EXPECT_TRUE(builder->redirectErrorStream());
}

} // namespace
