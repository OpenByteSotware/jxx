#include <gtest/gtest.h>
#include "jxx.initialize.h"
#include "nio/file/attribute/jxx.nio.file.attribute.ClassInitialization.h"
#include "nio/file/attribute/jxx.nio.file.attribute.FileTime.h"
namespace {
auto fileTimeClass()
{
    return ::jxx::lang::ClassBase<
        ::jxx::nio::file::attribute::FileTime,
        ::jxx::lang::Object,
        ::jxx::lang::Comparable<::jxx::nio::file::attribute::FileTime>>::Class();
}
}
TEST(JxxNioFileAttributeClassInitialization, FileTimeHasClassMetadata)
{
    ::jxx::initialize();
    EXPECT_NE(fileTimeClass(), nullptr);
}
TEST(JxxNioFileAttributeClassInitialization, RepeatedInitializationIsIdempotent)
{
    const auto before = fileTimeClass();
    EXPECT_NO_THROW(::jxx::nio::file::attribute::initializeClasses());
    EXPECT_NO_THROW(::jxx::nio::file::attribute::initializeClasses());
    EXPECT_EQ(before, fileTimeClass());
}
