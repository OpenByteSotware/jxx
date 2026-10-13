#include <gtest/gtest.h>

#include "lang/jxx.lang.ClassInfo.h"
#include "util/zip/jxx.util.zip.ClassInitialization.h"
#include "util/zip/jxx.util.zip.ZipEntry.h"
#include "util/zip/jxx.util.zip.ZipFile.h"

TEST(JxxZipRemainingClassInitialization, ZipEntryAndZipFileHaveMetadata)
{
    ::jxx::util::zip::initializeClasses();

    EXPECT_NE(
        ::jxx::util::zip::ZipEntry::JxxClassInfoMarker::Class(),
        nullptr);
    EXPECT_NE(
        ::jxx::util::zip::ZipFile::JxxClassInfoMarker::Class(),
        nullptr);
}

TEST(JxxZipRemainingClassInitialization, RepeatedInitializationIsIdempotent)
{
    const auto entryBefore =
        ::jxx::util::zip::ZipEntry::JxxClassInfoMarker::Class();
    const auto fileBefore =
        ::jxx::util::zip::ZipFile::JxxClassInfoMarker::Class();

    EXPECT_NO_THROW(::jxx::util::zip::initializeClasses());
    EXPECT_NO_THROW(::jxx::util::zip::initializeClasses());

    EXPECT_EQ(
        entryBefore,
        ::jxx::util::zip::ZipEntry::JxxClassInfoMarker::Class());
    EXPECT_EQ(
        fileBefore,
        ::jxx::util::zip::ZipFile::JxxClassInfoMarker::Class());
}
