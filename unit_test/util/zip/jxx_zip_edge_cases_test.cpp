#include <gtest/gtest.h>

#include <chrono>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.InputStream.h"
#include "io/jxx.io.OutputStream.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"
#include "util/zip/jxx.util.zip.CRC32.h"
#include "util/zip/jxx.util.zip.ZipEntry.h"
#include "util/zip/jxx.util.zip.ZipException.h"
#include "util/zip/jxx.util.zip.ZipFile.h"
#include "util/zip/jxx.util.zip.ZipOutputStream.h"

namespace {

using ::jxx::io::ByteArrayOutputStream;
using ::jxx::io::InputStream;
using ::jxx::io::OutputStream;
using ::jxx::lang::ByteArray;
using ::jxx::lang::ByteArrayType;
using ::jxx::lang::IllegalArgumentException;
using ::jxx::lang::IndexOutOfBoundsException;
using ::jxx::lang::NullPointerException;
using ::jxx::lang::String;
using ::jxx::util::zip::CRC32;
using ::jxx::util::zip::ZipEntry;
using ::jxx::util::zip::ZipException;
using ::jxx::util::zip::ZipFile;
using ::jxx::util::zip::ZipOutputStream;

class TemporaryZip final {
public:
    TemporaryZip() {
        const auto stamp = std::chrono::high_resolution_clock::now()
            .time_since_epoch().count();
        path_ = std::filesystem::temp_directory_path() /
            ("jxx_zip_edge_" + std::to_string(stamp) + ".zip");
    }

    ~TemporaryZip() {
        std::error_code error;
        std::filesystem::remove(path_, error);
    }

    const std::filesystem::path& path() const noexcept {
        return path_;
    }

private:
    std::filesystem::path path_;
};

ByteArray makeBytes(std::initializer_list<std::uint8_t> values) {
    auto result = ::jxx::NEW<ByteArrayType>(
        static_cast<::jxx::lang::jint>(values.size()));
    ::jxx::lang::jint index = 0;
    for (const auto value : values) {
        (*result)[index++] = static_cast<::jxx::lang::jbyte>(value);
    }
    return result;
}

std::uint16_t read16(const ByteArray& value, ::jxx::lang::jint offset) {
    return static_cast<std::uint16_t>(
        static_cast<std::uint8_t>((*value)[offset])) |
        static_cast<std::uint16_t>(
            static_cast<std::uint8_t>((*value)[offset + 1]) << 8U);
}

std::uint32_t read32(const ByteArray& value, ::jxx::lang::jint offset) {
    return static_cast<std::uint32_t>(read16(value, offset)) |
        (static_cast<std::uint32_t>(read16(value, offset + 2)) << 16U);
}

void write16(const ByteArray& value,
             ::jxx::lang::jint offset,
             std::uint16_t replacement) {
    (*value)[offset] = static_cast<::jxx::lang::jbyte>(replacement);
    (*value)[offset + 1] =
        static_cast<::jxx::lang::jbyte>(replacement >> 8U);
}

void write32(const ByteArray& value,
             ::jxx::lang::jint offset,
             std::uint32_t replacement) {
    write16(value, offset, static_cast<std::uint16_t>(replacement));
    write16(value, offset + 2,
            static_cast<std::uint16_t>(replacement >> 16U));
}

::jxx::lang::jint findSignature(const ByteArray& archive,
                                std::uint32_t signature) {
    for (::jxx::lang::jint index = 0; index <= archive->length - 4; ++index) {
        if (read32(archive, index) == signature) {
            return index;
        }
    }
    return -1;
}

void writeFile(const std::filesystem::path& path, const ByteArray& data) {
    std::ofstream output(path, std::ios::binary | std::ios::trunc);
    ASSERT_TRUE(output.good());
    for (::jxx::lang::jint index = 0; index < data->length; ++index) {
        output.put(static_cast<char>((*data)[index]));
    }
    ASSERT_TRUE(output.good());
}

ByteArray createArchive(const char* name,
                        const ByteArray& payload,
                        bool closeEntry = true) {
    auto sink = ::jxx::NEW<ByteArrayOutputStream>();
    auto zip = ::jxx::NEW<ZipOutputStream>(::jxx::CAST<OutputStream>(sink));
    zip->putNextEntry(::jxx::NEW<ZipEntry>(::jxx::NEW<String>(name)));
    zip->write(payload);
    if (closeEntry) {
        zip->closeEntry();
    }
    zip->finish();
    return sink->toByteArray();
}

std::vector<std::uint8_t> readAll(
    const ::jxx::Ptr<InputStream>& input)
{

    std::vector<std::uint8_t> result;
    auto buffer = ::jxx::NEW<ByteArrayType>(2);

    for (;;) {
        const auto count =
            input->read(
                buffer,
                0,
                buffer->length);

        if (count == -1) {
            break;
        }

        EXPECT_GE(count, 0);

        if (count < 0) {
            break;
        }

        for (::jxx::lang::jint index = 0;
             index < count;
             ++index) {

            result.push_back(
                static_cast<std::uint8_t>(
                    (*buffer)[index]));
        }
    }

    return result;
}


TEST(JxxZipEdgeCasesTest, EmptyArchiveIsReadable)
{
    auto sink = ::jxx::NEW<ByteArrayOutputStream>();
    auto output =
        ::jxx::NEW<ZipOutputStream>(::jxx::CAST<OutputStream>(sink));

    output->finish();

    TemporaryZip file;
    writeFile(file.path(), sink->toByteArray());

    auto zip = ::jxx::NEW<ZipFile>(
        ::jxx::NEW<String>(file.path().u8string()));

    EXPECT_EQ(zip->size(), 0);

    const auto entries = zip->entries();
    ASSERT_NE(entries, nullptr);
    EXPECT_FALSE(entries->hasMoreElements());
}

TEST(JxxZipEdgeCasesTest, EmptyDeflatedEntryRoundTrips) {
    TemporaryZip file;
    writeFile(file.path(), createArchive("empty.bin", makeBytes({})));
    auto zip = ::jxx::NEW<ZipFile>(::jxx::NEW<String>(file.path().u8string()));
    const auto entry = zip->getEntry(::jxx::NEW<String>("empty.bin"));
    ASSERT_NE(entry, nullptr);
    EXPECT_EQ(entry->getSize(), 0);
    EXPECT_TRUE(readAll(zip->getInputStream(entry)).empty());
}

TEST(JxxZipEdgeCasesTest, FinishClosesCurrentEntry) {
    TemporaryZip file;
    writeFile(file.path(), createArchive("implicit.txt", makeBytes({1, 2, 3}), false));
    auto zip = ::jxx::NEW<ZipFile>(::jxx::NEW<String>(file.path().u8string()));
    const auto entry = zip->getEntry(::jxx::NEW<String>("implicit.txt"));
    ASSERT_NE(entry, nullptr);
    EXPECT_EQ(readAll(zip->getInputStream(entry)),
              (std::vector<std::uint8_t>{1, 2, 3}));
}

TEST(JxxZipEdgeCasesTest, WriteWithoutCurrentEntryIsRejected) {
    auto sink = ::jxx::NEW<ByteArrayOutputStream>();
    auto zip = ::jxx::NEW<ZipOutputStream>(::jxx::CAST<OutputStream>(sink));
    EXPECT_THROW(zip->write(1), ZipException);
    EXPECT_THROW(zip->closeEntry(), ZipException);
}

TEST(JxxZipEdgeCasesTest, NullEntryAndNullBufferAreRejected) {
    auto sink = ::jxx::NEW<ByteArrayOutputStream>();
    auto zip = ::jxx::NEW<ZipOutputStream>(::jxx::CAST<OutputStream>(sink));
    EXPECT_THROW(zip->putNextEntry(nullptr), NullPointerException);

    zip->putNextEntry(::jxx::NEW<ZipEntry>(::jxx::NEW<String>("data.bin")));
    ByteArray nullBuffer;
    EXPECT_THROW(zip->write(nullBuffer), NullPointerException);
}

TEST(JxxZipEdgeCasesTest, InvalidWriteRangesAreRejected) {
    auto sink = ::jxx::NEW<ByteArrayOutputStream>();
    auto zip = ::jxx::NEW<ZipOutputStream>(::jxx::CAST<OutputStream>(sink));
    zip->putNextEntry(::jxx::NEW<ZipEntry>(::jxx::NEW<String>("data.bin")));
    const auto data = makeBytes({1, 2, 3});

    EXPECT_THROW(zip->write(data, -1, 1), IndexOutOfBoundsException);
    EXPECT_THROW(zip->write(data, 0, -1), IndexOutOfBoundsException);
    EXPECT_THROW(zip->write(data, 2, 2), IndexOutOfBoundsException);
}

TEST(JxxZipEdgeCasesTest, InvalidMethodAndCompressionLevelsAreRejected) {
    auto entry = ::jxx::NEW<ZipEntry>(::jxx::NEW<String>("data.bin"));
    EXPECT_THROW(entry->setMethod(7), IllegalArgumentException);

    auto sink = ::jxx::NEW<ByteArrayOutputStream>();
    auto zip = ::jxx::NEW<ZipOutputStream>(::jxx::CAST<OutputStream>(sink));
    EXPECT_THROW(zip->setMethod(7), IllegalArgumentException);
    EXPECT_THROW(zip->setLevel(-2), IllegalArgumentException);
    EXPECT_THROW(zip->setLevel(10), IllegalArgumentException);
    EXPECT_NO_THROW(zip->setLevel(-1));
    EXPECT_NO_THROW(zip->setLevel(0));
    EXPECT_NO_THROW(zip->setLevel(9));
}

TEST(JxxZipEdgeCasesTest, ZipEntryRejectsInvalidMetadata) {
    auto entry = ::jxx::NEW<ZipEntry>(::jxx::NEW<String>("data.bin"));
    EXPECT_THROW(entry->setSize(-1), IllegalArgumentException);
    EXPECT_THROW(entry->setCrc(-1), IllegalArgumentException);
    EXPECT_THROW(entry->setCrc(0x100000000LL), IllegalArgumentException);
    EXPECT_THROW(entry->setCompressedSize(-1), IllegalArgumentException);
}

TEST(JxxZipEdgeCasesTest, StoredEntryRejectsWrongSizeAndCrc) {
    const auto data = makeBytes({1, 2, 3, 4});
    auto sink = ::jxx::NEW<ByteArrayOutputStream>();
    auto zip = ::jxx::NEW<ZipOutputStream>(::jxx::CAST<OutputStream>(sink));
    auto entry = ::jxx::NEW<ZipEntry>(::jxx::NEW<String>("stored.bin"));
    entry->setMethod(ZipEntry::STORED);
    entry->setSize(3);
    entry->setCompressedSize(3);
    entry->setCrc(0);
    zip->putNextEntry(entry);
    zip->write(data);
    EXPECT_THROW(zip->closeEntry(), ZipException);
}

TEST(JxxZipEdgeCasesTest, CloseIsIdempotent) {
    auto sink = ::jxx::NEW<ByteArrayOutputStream>();
    auto zip = ::jxx::NEW<ZipOutputStream>(::jxx::CAST<OutputStream>(sink));
    zip->putNextEntry(::jxx::NEW<ZipEntry>(::jxx::NEW<String>("one.txt")));
    zip->write(makeBytes({1}));
    EXPECT_NO_THROW(zip->close());
    const auto sizeAfterFirstClose = sink->size();
    EXPECT_NO_THROW(zip->close());
    EXPECT_EQ(sink->size(), sizeAfterFirstClose);
    EXPECT_THROW(zip->putNextEntry(
        ::jxx::NEW<ZipEntry>(::jxx::NEW<String>("two.txt"))), ZipException);
}

TEST(JxxZipEdgeCasesTest, NullZipFileArgumentsAreRejected) {
    EXPECT_THROW(::jxx::NEW<ZipFile>(::jxx::Ptr<String>()), NullPointerException);
    EXPECT_THROW(::jxx::NEW<ZipEntry>(::jxx::Ptr<String>()), NullPointerException);
}

TEST(JxxZipEdgeCasesTest, EncryptedEntryIsRejected) {
    auto archive = createArchive("data.bin", makeBytes({1, 2, 3}));
    const auto central = findSignature(archive, 0x02014b50U);
    ASSERT_GE(central, 0);
    write16(archive, central + 8, static_cast<std::uint16_t>(read16(archive, central + 8) | 1U));

    TemporaryZip file;
    writeFile(file.path(), archive);
    EXPECT_THROW(::jxx::NEW<ZipFile>(
        ::jxx::NEW<String>(file.path().u8string())), ZipException);
}

TEST(JxxZipEdgeCasesTest, UnsupportedCompressionMethodIsRejected) {
    auto archive = createArchive("data.bin", makeBytes({1, 2, 3}));
    const auto central = findSignature(archive, 0x02014b50U);
    ASSERT_GE(central, 0);
    write16(archive, central + 10, 99);

    TemporaryZip file;
    writeFile(file.path(), archive);
    EXPECT_THROW(::jxx::NEW<ZipFile>(
        ::jxx::NEW<String>(file.path().u8string())), ZipException);
}

TEST(JxxZipEdgeCasesTest, MultiDiskArchiveIsRejected) {
    auto archive = createArchive("data.bin", makeBytes({1}));
    const auto end = findSignature(archive, 0x06054b50U);
    ASSERT_GE(end, 0);
    write16(archive, end + 4, 1);

    TemporaryZip file;
    writeFile(file.path(), archive);
    EXPECT_THROW(::jxx::NEW<ZipFile>(
        ::jxx::NEW<String>(file.path().u8string())), ZipException);
}

TEST(JxxZipEdgeCasesTest, Zip64MarkersAreRejectedExplicitly) {
    auto archive = createArchive("data.bin", makeBytes({1}));
    const auto end = findSignature(archive, 0x06054b50U);
    ASSERT_GE(end, 0);
    write16(archive, end + 10, 0xffffU);

    TemporaryZip file;
    writeFile(file.path(), archive);
    EXPECT_THROW(::jxx::NEW<ZipFile>(
        ::jxx::NEW<String>(file.path().u8string())), ZipException);
}

TEST(JxxZipEdgeCasesTest, LocalAndCentralMethodsMustMatch) {
    auto archive = createArchive("data.bin", makeBytes({1, 2, 3}));
    ASSERT_EQ(read32(archive, 0), 0x04034b50U);
    write16(archive, 8, ZipEntry::STORED);

    TemporaryZip file;
    writeFile(file.path(), archive);
    EXPECT_THROW(::jxx::NEW<ZipFile>(
        ::jxx::NEW<String>(file.path().u8string())), ZipException);
}

TEST(JxxZipEdgeCasesTest, LocalAndCentralNamesMustMatch) {
    auto archive = createArchive("same.bin", makeBytes({1, 2, 3}));
    ASSERT_EQ(read32(archive, 0), 0x04034b50U);
    ASSERT_GT(read16(archive, 26), 0U);
    (*archive)[30] = static_cast<::jxx::lang::jbyte>('x');

    TemporaryZip file;
    writeFile(file.path(), archive);
    EXPECT_THROW(::jxx::NEW<ZipFile>(
        ::jxx::NEW<String>(file.path().u8string())), ZipException);
}

TEST(JxxZipEdgeCasesTest, InvalidCentralDirectoryBoundsAreRejected) {
    auto archive = createArchive("data.bin", makeBytes({1}));
    const auto end = findSignature(archive, 0x06054b50U);
    ASSERT_GE(end, 0);
    write32(archive, end + 16, 0xffffff00U);

    TemporaryZip file;
    writeFile(file.path(), archive);
    EXPECT_THROW(::jxx::NEW<ZipFile>(
        ::jxx::NEW<String>(file.path().u8string())), ZipException);
}

TEST(JxxZipEdgeCasesTest, NullGetEntryAndGetInputStreamAreRejected) {
    TemporaryZip file;
    writeFile(file.path(), createArchive("data.bin", makeBytes({1})));
    auto zip = ::jxx::NEW<ZipFile>(::jxx::NEW<String>(file.path().u8string()));
    EXPECT_THROW(zip->getEntry(nullptr), NullPointerException);
    EXPECT_THROW(zip->getInputStream(nullptr), NullPointerException);
}

TEST(JxxZipEdgeCasesTest, ForeignEntryReturnsNullInputStream) {
    TemporaryZip file;
    writeFile(file.path(), createArchive("present.bin", makeBytes({1})));
    auto zip = ::jxx::NEW<ZipFile>(::jxx::NEW<String>(file.path().u8string()));
    auto foreign = ::jxx::NEW<ZipEntry>(::jxx::NEW<String>("missing.bin"));
    EXPECT_EQ(zip->getInputStream(foreign), nullptr);
}

TEST(JxxZipEdgeCasesTest, ExtraFieldAndCopyConstructorPreserveMetadata) {
    auto original = ::jxx::NEW<ZipEntry>(::jxx::NEW<String>("entry.bin"));
    original->setTime(1700000000000LL);
    original->setComment(::jxx::NEW<String>("comment"));
    original->setExtra(makeBytes({1, 2, 3, 4}));
    original->setMethod(ZipEntry::DEFLATED);

    auto copy = ::jxx::NEW<ZipEntry>(original);
    EXPECT_EQ(copy->getName()->utf8(), "entry.bin");
    EXPECT_EQ(copy->getTime(), original->getTime());
    ASSERT_NE(copy->getComment(), nullptr);
    EXPECT_EQ(copy->getComment()->utf8(), "comment");
    ASSERT_NE(copy->getExtra(), nullptr);
    EXPECT_EQ(copy->getExtra()->length, 4);
    EXPECT_EQ(copy->getMethod(), ZipEntry::DEFLATED);
}

} // namespace
