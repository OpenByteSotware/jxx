#include <gtest/gtest.h>
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.Runtime.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.System.h"
#include "lang/jxx.lang.UnsatisfiedLinkError.h"

namespace {
using ::jxx::lang::NullPointerException;
using ::jxx::lang::Runtime;
using ::jxx::lang::String;
using ::jxx::lang::System;
using ::jxx::lang::UnsatisfiedLinkError;

TEST(RuntimeUtilityParityTest, ProcessorAndMemoryValuesAreConsistent) {
    const auto runtime = Runtime::getRuntime();
    EXPECT_GE(runtime->availableProcessors(), 1);
    EXPECT_GE(runtime->totalMemory(), 0);
    EXPECT_GE(runtime->freeMemory(), 0);
    EXPECT_GE(runtime->maxMemory(), runtime->totalMemory());
}

TEST(SystemLibraryParityTest, MapLibraryNameUsesPlatformConvention) {
    const auto name = ::jxx::NEW<String>("example");
    const auto mapped = System::mapLibraryName(name);
#ifdef _WIN32
    EXPECT_EQ("example.dll", mapped->utf8());
#elif defined(__APPLE__)
    EXPECT_EQ("libexample.dylib", mapped->utf8());
#else
    EXPECT_EQ("libexample.so", mapped->utf8());
#endif
}

TEST(SystemLibraryParityTest, NullLibraryArgumentsAreRejected) {
    const ::jxx::Ptr<String> none;
    EXPECT_THROW(System::mapLibraryName(none), NullPointerException);
    EXPECT_THROW(System::load(none), NullPointerException);
    EXPECT_THROW(System::loadLibrary(none), NullPointerException);
}

TEST(RuntimeLibraryParityTest, LoadRequiresAnAbsoluteFilename) {
    const auto relative = ::jxx::NEW<String>("missing-library-file");
    EXPECT_THROW(Runtime::getRuntime()->load(relative), UnsatisfiedLinkError);
}

TEST(RuntimeLibraryParityTest, LoadLibraryRejectsPathComponents) {
    const auto pathName = ::jxx::NEW<String>("folder/library");
    EXPECT_THROW(Runtime::getRuntime()->loadLibrary(pathName), UnsatisfiedLinkError);
}
} // namespace
