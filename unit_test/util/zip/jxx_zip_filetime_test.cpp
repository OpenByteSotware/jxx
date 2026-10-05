#include <gtest/gtest.h>
#include "lang/jxx.lang.String.h"
#include "nio/file/attribute/jxx.nio.file.attribute.FileTime.h"
#include "util/zip/jxx.util.zip.ZipEntry.h"
namespace {TEST(JxxZipFileTimeTest, TimesRoundTrip){auto e=::jxx::NEW<::jxx::util::zip::ZipEntry>(::jxx::NEW<::jxx::lang::String>("x"));auto t=::jxx::nio::file::attribute::FileTime::fromMillis(123456789);e->setLastModifiedTime(t);e->setLastAccessTime(t);e->setCreationTime(t);EXPECT_EQ(123456789,e->getTime());EXPECT_EQ(123456789,e->getLastModifiedTime()->toMillis());EXPECT_EQ(123456789,e->getLastAccessTime()->toMillis());EXPECT_EQ(123456789,e->getCreationTime()->toMillis());}}
