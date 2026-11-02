#include <gtest/gtest.h>
#include <type_traits>

#include "io/jxx.io.Closeable.h"
#include "io/jxx.io.FilterInputStream.h"
#include "io/jxx.io.FilterOutputStream.h"
#include "lang/jxx.lang.Cloneable.h"
#include "lang/jxx.lang.Exception.h"
#include "lang/jxx.lang.Object.h"
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
#include "util/zip/jxx.util.zip.ZipEntry.h"
#include "util/zip/jxx.util.zip.ZipException.h"
#include "util/zip/jxx.util.zip.ZipFile.h"
#include "util/zip/jxx.util.zip.ZipInputStream.h"
#include "util/zip/jxx.util.zip.ZipOutputStream.h"

namespace zip = ::jxx::util::zip;

TEST(UtilZipInheritanceParity, ChecksumImplementationsMatchSourceInterfaces) {
    static_assert(std::is_base_of_v<zip::Checksum, zip::Adler32>);
    static_assert(std::is_base_of_v<zip::Checksum, zip::CRC32>);
    static_assert(std::is_base_of_v<::jxx::lang::Object, zip::Adler32>);
    static_assert(std::is_base_of_v<::jxx::lang::Object, zip::CRC32>);
    SUCCEED();
}

TEST(UtilZipInheritanceParity, StreamSuperclassesMatchSourceHierarchy) {
    static_assert(std::is_base_of_v<::jxx::io::FilterInputStream, zip::CheckedInputStream>);
    static_assert(std::is_base_of_v<::jxx::io::FilterOutputStream, zip::CheckedOutputStream>);
    static_assert(std::is_base_of_v<::jxx::io::FilterInputStream, zip::InflaterInputStream>);
    static_assert(std::is_base_of_v<::jxx::io::FilterOutputStream, zip::DeflaterOutputStream>);
    static_assert(std::is_base_of_v<zip::InflaterInputStream, zip::GZIPInputStream>);
    static_assert(std::is_base_of_v<zip::InflaterInputStream, zip::ZipInputStream>);
    static_assert(std::is_base_of_v<zip::DeflaterOutputStream, zip::GZIPOutputStream>);
    static_assert(std::is_base_of_v<zip::DeflaterOutputStream, zip::ZipOutputStream>);
    SUCCEED();
}

TEST(UtilZipInheritanceParity, EntryAndFileInterfacesMatchPublicSourceTypes) {
    static_assert(std::is_base_of_v<::jxx::lang::Cloneable, zip::ZipEntry>);
    static_assert(std::is_base_of_v<::jxx::io::Closeable, zip::ZipFile>);
    EXPECT_TRUE((std::is_base_of_v<::jxx::lang::Cloneable, zip::ZipEntry>));
}

TEST(UtilZipInheritanceParity, PublicConcreteTypesRemainExtensible) {
    static_assert(!std::is_final_v<zip::Adler32>);
    static_assert(!std::is_final_v<zip::CRC32>);
    static_assert(!std::is_final_v<zip::CheckedInputStream>);
    static_assert(!std::is_final_v<zip::CheckedOutputStream>);
    static_assert(!std::is_final_v<zip::Deflater>);
    static_assert(!std::is_final_v<zip::Inflater>);
    static_assert(!std::is_final_v<zip::ZipEntry>);
    static_assert(!std::is_final_v<zip::ZipFile>);
    static_assert(!std::is_final_v<zip::GZIPInputStream>);
    static_assert(!std::is_final_v<zip::GZIPOutputStream>);
    static_assert(!std::is_final_v<zip::ZipInputStream>);
    static_assert(!std::is_final_v<zip::ZipOutputStream>);
    SUCCEED();
}

TEST(UtilZipInheritanceParity, PackageTypesExposeClassMetadata) {
    EXPECT_NE(nullptr, zip::Adler32::Class());
    EXPECT_NE(nullptr, zip::CRC32::Class());
    EXPECT_NE(nullptr, zip::ZipEntry::Class());
    EXPECT_NE(nullptr, zip::ZipFile::Class());
    EXPECT_NE(nullptr, zip::ZipInputStream::Class());
    EXPECT_NE(nullptr, zip::ZipOutputStream::Class());
}
