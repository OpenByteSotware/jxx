#include <gtest/gtest.h>

#include <cstdint>
#include <vector>

#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.OutputStream.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"
#include "util/zip/jxx.util.zip.CRC32.h"
#include "util/zip/jxx.util.zip.ZipEntry.h"
#include "util/zip/jxx.util.zip.ZipException.h"
#include "util/zip/jxx.util.zip.ZipOutputStream.h"

namespace {

using ::jxx::io::ByteArrayOutputStream;
using ::jxx::io::OutputStream;
using ::jxx::lang::ByteArray;
using ::jxx::lang::ByteArrayType;
using ::jxx::lang::String;
using ::jxx::util::zip::CRC32;
using ::jxx::util::zip::ZipEntry;
using ::jxx::util::zip::ZipException;
using ::jxx::util::zip::ZipOutputStream;

ByteArray bytes(std::initializer_list<std::uint8_t> values) {
    auto result = ::jxx::NEW<ByteArrayType>(
        static_cast<::jxx::lang::jint>(values.size()));
    ::jxx::lang::jint index = 0;
    for (const auto value : values) {
        (*result)[index++] = static_cast<::jxx::lang::jbyte>(value);
    }
    return result;
}

std::uint16_t little16(const ByteArray& value, ::jxx::lang::jint offset) {
    return static_cast<std::uint16_t>(
        static_cast<std::uint8_t>((*value)[offset])) |
        static_cast<std::uint16_t>(
            static_cast<std::uint8_t>((*value)[offset + 1]) << 8U);
}

std::uint32_t little32(const ByteArray& value, ::jxx::lang::jint offset) {
    return static_cast<std::uint32_t>(little16(value, offset)) |
        (static_cast<std::uint32_t>(little16(value, offset + 2)) << 16U);
}

TEST(JxxZipOutputStreamTest, DefaultEntryIsDeflatedAndArchiveHasCentralDirectory) {
    auto sink = ::jxx::NEW<ByteArrayOutputStream>();
    auto zip = ::jxx::NEW<ZipOutputStream>(::jxx::CAST<OutputStream>(sink));
    auto entry = ::jxx::NEW<ZipEntry>(::jxx::NEW<String>("payload.bin"));
    const auto input = bytes({0, 1, 2, 3, 4, 5, 6, 7, 8, 9});

    zip->putNextEntry(entry);
    zip->write(input);
    zip->closeEntry();
    zip->finish();

    const auto archive = sink->toByteArray();
    ASSERT_NE(archive, nullptr);
    ASSERT_GT(archive->length, 30);
    EXPECT_EQ(little32(archive, 0), 0x04034b50U);
    EXPECT_EQ(little16(archive, 8), ZipEntry::DEFLATED);

    bool foundCentralDirectory = false;
    for (::jxx::lang::jint index = 0; index <= archive->length - 4; ++index) {
        if (little32(archive, index) == 0x02014b50U) {
            foundCentralDirectory = true;
            EXPECT_EQ(little16(archive, index + 10), ZipEntry::DEFLATED);
            break;
        }
    }
    EXPECT_TRUE(foundCentralDirectory);
}

TEST(JxxZipOutputStreamTest, StoredEntryRequiresSizeCompressedSizeAndCrc) {
    auto sink = ::jxx::NEW<ByteArrayOutputStream>();
    auto zip = ::jxx::NEW<ZipOutputStream>(::jxx::CAST<OutputStream>(sink));
    auto entry = ::jxx::NEW<ZipEntry>(::jxx::NEW<String>("stored.bin"));
    entry->setMethod(ZipEntry::STORED);

    zip->putNextEntry(entry);
    zip->write(bytes({1, 2, 3}));
    EXPECT_THROW(zip->closeEntry(), ZipException);
}

TEST(JxxZipOutputStreamTest, StoredEntryWithRequiredMetadataIsWrittenStored) {
    const auto input = bytes({9, 8, 7, 6});
    auto checksum = ::jxx::NEW<CRC32>();
    checksum->update(input);

    auto sink = ::jxx::NEW<ByteArrayOutputStream>();
    auto zip = ::jxx::NEW<ZipOutputStream>(::jxx::CAST<OutputStream>(sink));
    auto entry = ::jxx::NEW<ZipEntry>(::jxx::NEW<String>("stored.bin"));
    entry->setMethod(ZipEntry::STORED);
    entry->setSize(input->length);
    entry->setCompressedSize(input->length);
    entry->setCrc(checksum->getValue());

    zip->putNextEntry(entry);
    zip->write(input);
    zip->closeEntry();
    zip->finish();

    const auto archive = sink->toByteArray();
    ASSERT_GT(archive->length, 30);
    EXPECT_EQ(little16(archive, 8), ZipEntry::STORED);
    EXPECT_EQ(little32(archive, 18), static_cast<std::uint32_t>(input->length));
    EXPECT_EQ(little32(archive, 22), static_cast<std::uint32_t>(input->length));
}

TEST(JxxZipOutputStreamTest, DuplicateEntryNameIsRejected) {
    auto sink = ::jxx::NEW<ByteArrayOutputStream>();
    auto zip = ::jxx::NEW<ZipOutputStream>(::jxx::CAST<OutputStream>(sink));

    zip->putNextEntry(::jxx::NEW<ZipEntry>(::jxx::NEW<String>("same.txt")));
    zip->write(bytes({1}));
    zip->closeEntry();

    EXPECT_THROW(
        zip->putNextEntry(::jxx::NEW<ZipEntry>(::jxx::NEW<String>("same.txt"))),
        ZipException);
}

TEST(JxxZipOutputStreamTest, FinishIsIdempotentAndFurtherWritesAreRejected) {
    auto sink = ::jxx::NEW<ByteArrayOutputStream>();
    auto zip = ::jxx::NEW<ZipOutputStream>(::jxx::CAST<OutputStream>(sink));

    zip->putNextEntry(::jxx::NEW<ZipEntry>(::jxx::NEW<String>("one.txt")));
    zip->write(bytes({1, 2, 3}));
    zip->finish();
    const auto firstSize = sink->size();

    EXPECT_NO_THROW(zip->finish());
    EXPECT_EQ(sink->size(), firstSize);
    EXPECT_THROW(zip->write(1), ZipException);
}

TEST(JxxZipOutputStreamTest, EntryCommentArchiveCommentAndTimestampAreEncoded) {
    auto sink = ::jxx::NEW<ByteArrayOutputStream>();
    auto zip = ::jxx::NEW<ZipOutputStream>(::jxx::CAST<OutputStream>(sink));
    zip->setComment(::jxx::NEW<String>("archive-comment"));

    auto entry = ::jxx::NEW<ZipEntry>(::jxx::NEW<String>("time.txt"));
    entry->setComment(::jxx::NEW<String>("entry-comment"));
    entry->setTime(1700000000000LL);
    zip->putNextEntry(entry);
    zip->write(bytes({42}));
    zip->closeEntry();
    zip->finish();

    const auto archive = sink->toByteArray();
    EXPECT_NE(little16(archive, 10), 0U);
    EXPECT_NE(little16(archive, 12), 0U);
}

} // namespace
