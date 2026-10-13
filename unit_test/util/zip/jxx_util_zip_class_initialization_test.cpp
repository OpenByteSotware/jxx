#include <gtest/gtest.h>

#include "jxx.initialize.h"
#include "util/zip/jxx.util.zip.ClassInitialization.h"
#include "util/zip/jxx.util.zip.Adler32.h"
#include "util/zip/jxx.util.zip.CRC32.h"
#include "util/zip/jxx.util.zip.CheckedInputStream.h"
#include "util/zip/jxx.util.zip.CheckedOutputStream.h"
#include "util/zip/jxx.util.zip.Checksum.h"
#include "util/zip/jxx.util.zip.DataFormatException.h"
#include "util/zip/jxx.util.zip.Deflater.h"
#include "util/zip/jxx.util.zip.DeflaterOutputStream.h"
#include "util/zip/jxx.util.zip.GZIPInputStream.h"
#include "util/zip/jxx.util.zip.GZIPOutputStream.h"
#include "util/zip/jxx.util.zip.Inflater.h"
#include "util/zip/jxx.util.zip.InflaterInputStream.h"
#include "util/zip/jxx.util.zip.ZipException.h"
#include "util/zip/jxx.util.zip.ZipInputStream.h"
#include "util/zip/jxx.util.zip.ZipOutputStream.h"

TEST(JxxUtilZipClassInitialization, MetadataTypesAreInitialized)
{
    ::jxx::initialize();

    EXPECT_NE(::jxx::util::zip::Adler32::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(::jxx::util::zip::CRC32::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(::jxx::util::zip::CheckedInputStream::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(::jxx::util::zip::CheckedOutputStream::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(::jxx::util::zip::Checksum::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(::jxx::util::zip::DataFormatException::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(::jxx::util::zip::Deflater::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(::jxx::util::zip::DeflaterOutputStream::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(::jxx::util::zip::GZIPInputStream::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(::jxx::util::zip::GZIPOutputStream::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(::jxx::util::zip::Inflater::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(::jxx::util::zip::InflaterInputStream::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(::jxx::util::zip::ZipException::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(::jxx::util::zip::ZipInputStream::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(::jxx::util::zip::ZipOutputStream::JxxClassInfoMarker::Class(), nullptr);
}

TEST(JxxUtilZipClassInitialization, RepeatedInitializationIsIdempotent)
{
    const auto checksumBefore =
        ::jxx::util::zip::Checksum::JxxClassInfoMarker::Class();
    const auto zipExceptionBefore =
        ::jxx::util::zip::ZipException::JxxClassInfoMarker::Class();

    EXPECT_NO_THROW(::jxx::util::zip::initializeClasses());
    EXPECT_NO_THROW(::jxx::util::zip::initializeClasses());

    EXPECT_EQ(
        checksumBefore,
        ::jxx::util::zip::Checksum::JxxClassInfoMarker::Class());
    EXPECT_EQ(
        zipExceptionBefore,
        ::jxx::util::zip::ZipException::JxxClassInfoMarker::Class());
}
