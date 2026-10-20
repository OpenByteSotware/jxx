#include <gtest/gtest.h>

#include "jxx.initialize.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.ProcessBuilder.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.ArrayList.h"
#include "util/jxx.util.List.h"

namespace {

using CommandArray =
    ::jxx::lang::JxxArray<
        ::jxx::Ptr<::jxx::lang::String>,
        1U>;

using CommandList =
    ::jxx::util::List<
        ::jxx::lang::String>;

auto value(const char* text)
    -> ::jxx::Ptr<::jxx::lang::String>
{
    return ::jxx::NEW<::jxx::lang::String>(text);
}

TEST(
    ProcessBuilderListParity,
    ConstructsFromListAndCopiesElements)
{
    ::jxx::initialize();

    auto command = ::jxx::NEW<
        ::jxx::util::ArrayList<
            ::jxx::lang::String>>();

    command->add(value("tool"));
    command->add(value("first"));

    auto builder =
        ::jxx::NEW<::jxx::lang::ProcessBuilder>(
            command);

    command->set(1, value("changed"));

    auto stored = builder->command();

    ASSERT_EQ(stored->size(), 2U);
    EXPECT_EQ((*stored)[0]->utf8(), "tool");
    EXPECT_EQ((*stored)[1]->utf8(), "first");
}

TEST(
    ProcessBuilderListParity,
    CommandListReplacesAndCopiesElements)
{
    ::jxx::initialize();

    auto initial = ::jxx::NEW<CommandArray>(1);
    (*initial)[0] = value("initial");

    auto builder =
        ::jxx::NEW<::jxx::lang::ProcessBuilder>(
            initial);

    auto replacement = ::jxx::NEW<
        ::jxx::util::ArrayList<
            ::jxx::lang::String>>();

    replacement->add(value("sh"));
    replacement->add(value("-c"));
    replacement->add(value("echo parity"));

    EXPECT_EQ(
        builder->command(replacement),
        builder);

    replacement->set(2, value("changed"));

    auto stored = builder->command();

    ASSERT_EQ(stored->size(), 3U);
    EXPECT_EQ((*stored)[0]->utf8(), "sh");
    EXPECT_EQ((*stored)[1]->utf8(), "-c");
    EXPECT_EQ((*stored)[2]->utf8(), "echo parity");
}

TEST(
    ProcessBuilderListParity,
    RejectsNullList)
{
    ::jxx::initialize();

    const ::jxx::Ptr<CommandList> nullList =
        nullptr;

    EXPECT_THROW(
        ::jxx::NEW<::jxx::lang::ProcessBuilder>(
            nullList),
        ::jxx::lang::NullPointerException);
}

TEST(
    ProcessBuilderListParity,
    RejectsNullArray)
{
    ::jxx::initialize();

    const ::jxx::Ptr<CommandArray> nullArray =
        nullptr;

    EXPECT_THROW(
        ::jxx::NEW<::jxx::lang::ProcessBuilder>(
            nullArray),
        ::jxx::lang::NullPointerException);
}

TEST(
    ProcessBuilderListParity,
    RejectsNullListElement)
{
    ::jxx::initialize();

    auto command = ::jxx::NEW<
        ::jxx::util::ArrayList<
            ::jxx::lang::String>>();

    command->add(value("tool"));
    command->add(nullptr);

    EXPECT_THROW(
        ::jxx::NEW<::jxx::lang::ProcessBuilder>(
            command),
        ::jxx::lang::NullPointerException);
}

TEST(
    ProcessBuilderListParity,
    RejectsNullArrayElement)
{
    ::jxx::initialize();

    auto command = ::jxx::NEW<CommandArray>(2);

    (*command)[0] = value("tool");
    (*command)[1] = nullptr;

    EXPECT_THROW(
        ::jxx::NEW<::jxx::lang::ProcessBuilder>(
            command),
        ::jxx::lang::NullPointerException);
}

TEST(
    ProcessBuilderListParity,
    CommandListRejectsNullList)
{
    ::jxx::initialize();

    auto initial = ::jxx::NEW<CommandArray>(1);
    (*initial)[0] = value("initial");

    auto builder =
        ::jxx::NEW<::jxx::lang::ProcessBuilder>(
            initial);

    const ::jxx::Ptr<CommandList> nullList =
        nullptr;

    EXPECT_THROW(
        builder->command(nullList),
        ::jxx::lang::NullPointerException);
}

TEST(
    ProcessBuilderListParity,
    CommandListRejectsNullElement)
{
    ::jxx::initialize();

    auto initial = ::jxx::NEW<CommandArray>(1);
    (*initial)[0] = value("initial");

    auto builder =
        ::jxx::NEW<::jxx::lang::ProcessBuilder>(
            initial);

    auto replacement = ::jxx::NEW<
        ::jxx::util::ArrayList<
            ::jxx::lang::String>>();

    replacement->add(value("tool"));
    replacement->add(nullptr);

    EXPECT_THROW(
        builder->command(replacement),
        ::jxx::lang::NullPointerException);

    // A rejected replacement must not change the existing command.
    auto stored = builder->command();

    ASSERT_EQ(stored->size(), 1U);
    EXPECT_EQ((*stored)[0]->utf8(), "initial");
}

} // namespace