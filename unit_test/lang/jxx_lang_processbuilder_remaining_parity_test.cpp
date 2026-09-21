#include <gtest/gtest.h>

#include <string>

#include "io/jxx.io.File.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.ProcessBuilder.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"

namespace {
using ::jxx::lang::IllegalArgumentException;
using ::jxx::lang::IndexOutOfBoundsException;
using ::jxx::lang::NullPointerException;
using ::jxx::lang::ProcessBuilder;
using ::jxx::lang::String;

auto command(std::initializer_list<const char*> values) {
    auto result = ::jxx::NEW<
        ::jxx::lang::JxxArray<::jxx::Ptr<String>, 1U>>(
            static_cast<std::uint32_t>(values.size()));
    ::jxx::lang::jint index = 0;
    for (const auto* value : values) {
        (*result)[index++] = ::jxx::NEW<String>(value);
    }
    return result;
}

TEST(ProcessBuilderRemainingParityTest, NullCommandIsRejectedButEmptyCommandIsStored) {
    const ::jxx::Ptr<
        ::jxx::lang::JxxArray<::jxx::Ptr<String>, 1U>> nullCommand;
    EXPECT_THROW(::jxx::NEW<ProcessBuilder>(nullCommand), NullPointerException);

    const auto empty = ::jxx::NEW<
        ::jxx::lang::JxxArray<::jxx::Ptr<String>, 1U>>(0U);
    const auto builder = ::jxx::NEW<ProcessBuilder>(empty);
    ASSERT_NE(nullptr, builder->command());
    EXPECT_EQ(0U, builder->command()->length);
    EXPECT_THROW(builder->start(), IndexOutOfBoundsException);
}

TEST(ProcessBuilderRemainingParityTest, CommandSetterIsAtomicWhenAnElementIsNull) {
    const auto builder = ::jxx::NEW<ProcessBuilder>(command({"tool", "old"}));
    const auto replacement = command({"tool", "new"});
    (*replacement)[1] = nullptr;

    EXPECT_THROW(builder->command(replacement), NullPointerException);
    const auto retained = builder->command();
    ASSERT_EQ(2U, retained->length);
    EXPECT_EQ("old", (*retained)[1]->utf8());
}

TEST(ProcessBuilderRemainingParityTest, EnvironmentValidatesNamesAndValuesOnMutation) {
    const auto builder = ::jxx::NEW<ProcessBuilder>(command({"tool"}));
    const ::jxx::Ptr<String> nullString;
    EXPECT_THROW(builder->environment(nullString, ::jxx::NEW<String>("value")),
                 NullPointerException);
    EXPECT_THROW(builder->environment(::jxx::NEW<String>("NAME"), nullString),
                 NullPointerException);
    EXPECT_THROW(builder->environment(::jxx::NEW<String>(""),
                                      ::jxx::NEW<String>("value")),
                 IllegalArgumentException);
    EXPECT_THROW(builder->environment(::jxx::NEW<String>("A=B"),
                                      ::jxx::NEW<String>("value")),
                 IllegalArgumentException);
    EXPECT_THROW(builder->environment(
                     ::jxx::NEW<String>(std::string("A\0B", 3)),
                     ::jxx::NEW<String>("value")),
                 IllegalArgumentException);
    EXPECT_THROW(builder->environment(
                     ::jxx::NEW<String>("NAME"),
                     ::jxx::NEW<String>(std::string("A\0B", 3))),
                 IllegalArgumentException);

    EXPECT_EQ(builder.get(),
              builder->environment(::jxx::NEW<String>("NAME"),
                                   ::jxx::NEW<String>("value")).get());
    EXPECT_EQ("value",
              builder->environment(::jxx::NEW<String>("NAME"))->utf8());
    builder->clearEnvironment();
    EXPECT_EQ(nullptr, builder->environment(::jxx::NEW<String>("NAME")));
}

TEST(ProcessBuilderRemainingParityTest, RedirectFactoriesExposeTypeFileEqualityAndHashing) {
    const auto firstFile = ::jxx::NEW<::jxx::io::File>(
        ::jxx::NEW<String>("process-output.log"));
    const auto equivalentFile = ::jxx::NEW<::jxx::io::File>(
        ::jxx::NEW<String>("process-output.log"));
    const auto otherFile = ::jxx::NEW<::jxx::io::File>(
        ::jxx::NEW<String>("other-output.log"));

    const auto first = ProcessBuilder::Redirect::appendTo(firstFile);
    const auto equivalent = ProcessBuilder::Redirect::appendTo(equivalentFile);
    const auto write = ProcessBuilder::Redirect::to(firstFile);
    const auto other = ProcessBuilder::Redirect::appendTo(otherFile);

    EXPECT_EQ(ProcessBuilder::Redirect::Type::APPEND, first->type());
    EXPECT_EQ(firstFile.get(), first->file().get());
    EXPECT_TRUE(first->equals(::jxx::CAST<::jxx::lang::Object>(equivalent)));
    EXPECT_EQ(first->hashCode(), equivalent->hashCode());
    EXPECT_FALSE(first->equals(::jxx::CAST<::jxx::lang::Object>(write)));
    EXPECT_FALSE(first->equals(::jxx::CAST<::jxx::lang::Object>(other)));
    EXPECT_EQ(nullptr, ProcessBuilder::Redirect::PIPE->file());
    EXPECT_EQ(nullptr, ProcessBuilder::Redirect::INHERIT->file());
}

TEST(ProcessBuilderRemainingParityTest, RedirectDirectionRulesRejectInvalidTypes) {
    const auto file = ::jxx::NEW<::jxx::io::File>(
        ::jxx::NEW<String>("redirect.log"));
    const auto builder = ::jxx::NEW<ProcessBuilder>(command({"tool"}));

    EXPECT_THROW(builder->redirectInput(ProcessBuilder::Redirect::to(file)),
                 IllegalArgumentException);
    EXPECT_THROW(builder->redirectInput(ProcessBuilder::Redirect::appendTo(file)),
                 IllegalArgumentException);
    EXPECT_THROW(builder->redirectOutput(ProcessBuilder::Redirect::from(file)),
                 IllegalArgumentException);
    EXPECT_THROW(builder->redirectError(ProcessBuilder::Redirect::from(file)),
                 IllegalArgumentException);
}

} // namespace
