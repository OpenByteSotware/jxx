#include <gtest/gtest.h>

#include <atomic>
#include <cstdint>
#include <initializer_list>

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IllegalThreadStateException.h"
#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.Process.h"
#include "lang/jxx.lang.ProcessBuilder.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"
#include "util/concurrent/jxx.util.concurrent.TimeUnit.h"

namespace
{

    using ::jxx::lang::IllegalArgumentException;
    using ::jxx::lang::IllegalThreadStateException;
    using ::jxx::lang::NullPointerException;
    using ::jxx::lang::Process;
    using ::jxx::lang::ProcessBuilder;
    using ::jxx::lang::String;
    using ::jxx::util::concurrent::TimeUnit;

    using CommandArray =
        ::jxx::lang::JxxArray<
        ::jxx::Ptr<String>,
        1U>;

    using CommandArrayPtr =
        ::jxx::Ptr<CommandArray>;

    class FakeProcess final : public Process
    {
    public:
        using Process::waitFor;

        std::atomic<bool> alive{ true };
        std::atomic<bool> destroyed{ false };

        ::jxx::Ptr<::jxx::io::OutputStream>
            getOutputStream() override
        {
            return nullptr;
        }

        ::jxx::Ptr<::jxx::io::InputStream>
            getInputStream() override
        {
            return nullptr;
        }

        ::jxx::Ptr<::jxx::io::InputStream>
            getErrorStream() override
        {
            return nullptr;
        }

        ::jxx::lang::jint waitFor() override
        {
            alive = false;
            return 0;
        }

        ::jxx::lang::jint exitValue() override
        {
            if (alive) {
                throw IllegalThreadStateException();
            }

            return 0;
        }

        void destroy() override
        {
            destroyed = true;
            alive = false;
        }
    };

    auto command(
        std::initializer_list<const char*> values)
        -> CommandArrayPtr
    {
        auto result = ::jxx::NEW<CommandArray>(
            static_cast<std::uint32_t>(values.size()));

        ::jxx::lang::jint index = 0;

        for (const auto* commandValue : values) {
            (*result)[index++] =
                ::jxx::NEW<String>(commandValue);
        }

        return result;
    }

    TEST(
        ProcessEdgeTest,
        TimedWaitValidatesUnitAndHandlesNonPositiveTimeout)
    {
        const auto process =
            ::jxx::NEW<FakeProcess>();

        EXPECT_THROW(
            process->waitFor(
                1,
                ::jxx::Ptr<TimeUnit>{}),
            NullPointerException);

        EXPECT_FALSE(
            process->waitFor(
                0,
                TimeUnit::MILLISECONDS()));

        EXPECT_FALSE(
            process->waitFor(
                -1,
                TimeUnit::MILLISECONDS()));

        process->alive = false;

        EXPECT_TRUE(
            process->waitFor(
                0,
                TimeUnit::MILLISECONDS()));
    }

    TEST(
        ProcessEdgeTest,
        DefaultLivenessAndForcefulDestroyUseCoreHooks)
    {
        const auto process =
            ::jxx::NEW<FakeProcess>();

        EXPECT_TRUE(process->isAlive());

        EXPECT_EQ(
            process.get(),
            process->destroyForcibly().get());

        EXPECT_TRUE(process->destroyed.load());
        EXPECT_FALSE(process->isAlive());
    }

    TEST(
        ProcessBuilderEdgeTest,
        ConstructorCopiesCommandAndGetterReturnsCopy)
    {
        const auto original =
            command({ "tool", "one" });

        const auto builder =
            ::jxx::NEW<ProcessBuilder>(original);

        (*original)[1] =
            ::jxx::NEW<String>("changed");

        auto first = builder->command();

        ASSERT_NE(first, nullptr);
        ASSERT_EQ(first->size(), 2U);
        EXPECT_EQ((*first)[0]->utf8(), "tool");
        EXPECT_EQ((*first)[1]->utf8(), "one");

        (*first)[1] =
            ::jxx::NEW<String>("mutated");

        auto second = builder->command();

        ASSERT_NE(second, nullptr);
        ASSERT_EQ(second->size(), 2U);
        EXPECT_EQ((*second)[0]->utf8(), "tool");
        EXPECT_EQ((*second)[1]->utf8(), "one");
    }

    TEST(
        ProcessBuilderEdgeTest,
        RejectsEmptyNullAndInvalidRedirectConfiguration)
    {
        // A typed null is required because ProcessBuilder has both
        // command-array and List<String> constructors. Passing raw nullptr
        // would be ambiguous and would fail jxx::NEW's constructibility check.
        const CommandArrayPtr nullCommand = nullptr;

        EXPECT_THROW(
            ::jxx::NEW<ProcessBuilder>(
                nullCommand),
            NullPointerException);

        const auto empty =
            ::jxx::NEW<CommandArray>(0U);

        const auto emptyBuilder =
            ::jxx::NEW<ProcessBuilder>(empty);

        ASSERT_NE(emptyBuilder, nullptr);

        EXPECT_THROW(
            emptyBuilder->start(),
            ::jxx::lang::IndexOutOfBoundsException);

        auto withNull =
            command({ "tool", "argument" });

        (*withNull)[1] = nullptr;

        EXPECT_THROW(
            ::jxx::NEW<ProcessBuilder>(
                withNull),
            NullPointerException);

        const auto builder =
            ::jxx::NEW<ProcessBuilder>(
                command({ "tool" }));

        EXPECT_THROW(
            builder->redirectInput(
                ::jxx::Ptr<
                ProcessBuilder::Redirect>{}),
            NullPointerException);

        EXPECT_THROW(
            builder->redirectOutput(
                ProcessBuilder::Redirect::from(
                    ::jxx::Ptr<
                    ::jxx::io::File>{})),
            NullPointerException);
    }

    TEST(
        ProcessBuilderEdgeTest,
        InheritIoAndErrorStreamSettingsAreRetained)
    {
        const auto builder =
            ::jxx::NEW<ProcessBuilder>(
                command({ "tool" }));

        builder
            ->inheritIO()
            ->redirectErrorStream(true);

        EXPECT_EQ(
            ProcessBuilder::Redirect::Type::INHERIT,
            builder->redirectInput()->type());

        EXPECT_EQ(
            ProcessBuilder::Redirect::Type::INHERIT,
            builder->redirectOutput()->type());

        EXPECT_EQ(
            ProcessBuilder::Redirect::Type::INHERIT,
            builder->redirectError()->type());

        EXPECT_TRUE(
            builder->redirectErrorStream());
    }

} // namespace