#include <gtest/gtest.h>

#include "lang/jxx.lang.Compiler.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.StackTraceElement.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.StringBuilder.h"

namespace {
using ::jxx::lang::Compiler;
using ::jxx::lang::NullPointerException;
using ::jxx::lang::StackTraceElement;
using ::jxx::lang::String;
using ::jxx::lang::StringBuilder;

TEST(StringBuilderParityTest, CapacityUnicodeAndFluentMutation) {
    const auto builder = ::jxx::NEW<StringBuilder>();
    EXPECT_EQ(16, builder->capacity());
    builder->ensureCapacity(40);
    EXPECT_GE(builder->capacity(), 40);
    builder->append(::jxx::NEW<String>("A"));
    builder->appendCodePoint(0x1F600);
    builder->append(::jxx::NEW<String>("B"));
    EXPECT_EQ(4, builder->length());
    builder->reverse();
    EXPECT_EQ(std::u16string({u'B', 0xD83D, 0xDE00, u'A'}),
              builder->toString()->utf16());
}

TEST(StringBuilderParityTest, LengthGrowthPadsWithNullCharacters) {
    const auto builder = ::jxx::NEW<StringBuilder>(::jxx::NEW<String>("x"));
    builder->setLength(3);
    EXPECT_EQ(3, builder->length());
    EXPECT_EQ(0, builder->charAt(1));
    EXPECT_EQ(0, builder->charAt(2));
    builder->trimToSize();
    EXPECT_EQ(builder->length(), builder->capacity());
}

TEST(StackTraceElementParityTest, FormatsKnownUnknownAndNativeLocations) {
    const auto known = ::jxx::NEW<StackTraceElement>(
        ::jxx::NEW<String>("sample.Type"), ::jxx::NEW<String>("run"),
        ::jxx::NEW<String>("Type.cpp"), 42);
    EXPECT_EQ("sample.Type.run(Type.cpp:42)", known->toString()->utf8());

    const ::jxx::Ptr<String> noFile;
    const auto unknown = ::jxx::NEW<StackTraceElement>(
        ::jxx::NEW<String>("sample.Type"), ::jxx::NEW<String>("run"),
        noFile, -1);
    EXPECT_EQ("sample.Type.run(Unknown Source)", unknown->toString()->utf8());

    const auto nativeFrame = ::jxx::NEW<StackTraceElement>(
        ::jxx::NEW<String>("sample.Type"), ::jxx::NEW<String>("run"),
        noFile, -2);
    EXPECT_TRUE(nativeFrame->isNativeMethod());
    EXPECT_EQ("sample.Type.run(Native Method)", nativeFrame->toString()->utf8());
}

TEST(StackTraceElementParityTest, ConstructorRejectsRequiredNullFields) {
    const ::jxx::Ptr<String> none;
    const auto typeName = ::jxx::NEW<String>("sample.Type");
    const auto methodName = ::jxx::NEW<String>("run");
    EXPECT_THROW(StackTraceElement(none, methodName, none, 1), NullPointerException);
    EXPECT_THROW(StackTraceElement(typeName, none, none, 1), NullPointerException);
}

TEST(CompilerParityTest, NoCompilerServiceReturnsJavaCompatibleDefaults) {
    const auto type = String::Class();
    const auto pattern = ::jxx::NEW<String>("jxx.lang.*");
    const auto argument = ::jxx::CAST<::jxx::lang::Object>(pattern);
    EXPECT_FALSE(Compiler::compileClass(type));
    EXPECT_FALSE(Compiler::compileClasses(pattern));
    EXPECT_EQ(nullptr, Compiler::command(argument));
    Compiler::disable();
    Compiler::enable();
}

} // namespace
