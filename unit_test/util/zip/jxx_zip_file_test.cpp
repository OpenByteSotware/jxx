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
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"
#include "util/jxx.util.Enumeration.h"
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
using ::jxx::lang::String;
using ::jxx::util::zip::ZipEntry;
using ::jxx::util::zip::ZipException;
using ::jxx::util::zip::ZipFile;
using ::jxx::util::zip::ZipOutputStream;

class TemporaryFile final {
public:
    TemporaryFile() {
        const auto stamp = std::chrono::high_resolution_clock::now()
            .time_since_epoch().count();
        path_ = std::filesystem::temp_directory_path() /
            ("jxx_zip_test_" + std::to_string(stamp) + ".zip");
    }

    ~TemporaryFile() {
        std::error_code error;
        std::filesystem::remove(path_, error);
    }

    const std::filesystem::path& path() const noexcept {
        return path_;
    }

private:
    std::filesystem::path path_;
};

ByteArray bytes(std::initializer_list<std::uint8_t> values) {
    auto result = ::jxx::NEW<ByteArrayType>(
        static_cast<::jxx::lang::jint>(values.size()));
    ::jxx::lang::jint index = 0;
    for (const auto value : values) {
        (*result)[index++] = static_cast<::jxx::lang::jbyte>(value);
    }
    return result;
}

void writeNativeFile(const std::filesystem::path& path, const ByteArray& data) {
    std::ofstream output(path, std::ios::binary | std::ios::trunc);
    ASSERT_TRUE(output.good());
    for (::jxx::lang::jint index = 0; index < data->length; ++index) {
        output.put(static_cast<char>((*data)[index]));
    }
    ASSERT_TRUE(output.good());
}

ByteArray makeArchive() {
    auto sink = ::jxx::NEW<ByteArrayOutputStream>();
    auto zip = ::jxx::NEW<ZipOutputStream>(::jxx::CAST<OutputStream>(sink));
    zip->setComment(::jxx::NEW<String>("archive-comment"));

    auto first = ::jxx::NEW<ZipEntry>(::jxx::NEW<String>("folder/first.bin"));
    first->setComment(::jxx::NEW<String>("first-comment"));
    first->setTime(1700000000000LL);
    zip->putNextEntry(first);
    zip->write(bytes({0, 1, 2, 3, 4, 255}));
    zip->closeEntry();

    auto directory = ::jxx::NEW<ZipEntry>(::jxx::NEW<String>("empty/"));
    zip->putNextEntry(directory);
    zip->closeEntry();

    zip->putNextEntry(::jxx::NEW<ZipEntry>(::jxx::NEW<String>("second.txt")));
    zip->write(bytes({'J', 'X', 'X'}));
    zip->closeEntry();
    zip->finish();
    return sink->toByteArray();
}

std::vector<std::uint8_t> readAll(const ::jxx::Ptr<InputStream>& input) {
    std::vector<std::uint8_t> result;
    auto buffer = ::jxx::NEW<ByteArrayType>(3);
    for (;;) {
        const auto count = input->read(buffer, 0, buffer->length);
        if (count == -1) {
            break;
        }
        for (::jxx::lang::jint index = 0; index < count; ++index) {
            result.push_back(static_cast<std::uint8_t>((*buffer)[index]));
        }
    }
    return result;
}

TEST(JxxZipFileTest, EnumeratesEntriesAndReadsDeflatedContent) {
    TemporaryFile file;
    writeNativeFile(file.path(), makeArchive());

    auto zip = ::jxx::NEW<ZipFile>(
        ::jxx::NEW<String>(file.path().u8string()));
    EXPECT_EQ(zip->size(), 3);
    ASSERT_NE(zip->getComment(), nullptr);
    EXPECT_EQ(zip->getComment()->utf8(), "archive-comment");

    auto entries = zip->entries();
    ASSERT_TRUE(entries->hasMoreElements());
    const auto first = entries->nextElement();
    EXPECT_EQ(first->getName()->utf8(), "folder/first.bin");
    EXPECT_EQ(first->getMethod(), ZipEntry::DEFLATED);
    EXPECT_FALSE(first->isDirectory());
    ASSERT_NE(first->getComment(), nullptr);
    EXPECT_EQ(first->getComment()->utf8(), "first-comment");
    EXPECT_GT(first->getTime(), 0);

    const auto content = readAll(zip->getInputStream(first));
    EXPECT_EQ(content, (std::vector<std::uint8_t>{0, 1, 2, 3, 4, 255}));

    ASSERT_TRUE(entries->hasMoreElements());
    EXPECT_TRUE(entries->nextElement()->isDirectory());
    ASSERT_TRUE(entries->hasMoreElements());
    EXPECT_EQ(entries->nextElement()->getName()->utf8(), "second.txt");
    EXPECT_FALSE(entries->hasMoreElements());
}

TEST(JxxZipFileTest, GetEntryReturnsNullForMissingName) {
    TemporaryFile file;
    writeNativeFile(file.path(), makeArchive());
    auto zip = ::jxx::NEW<ZipFile>(::jxx::NEW<String>(file.path().u8string()));

    EXPECT_EQ(zip->getEntry(::jxx::NEW<String>("missing.txt")), nullptr);
}

TEST(JxxZipFileTest, ClosedArchiveRejectsEntryOperations) {
    TemporaryFile file;
    writeNativeFile(file.path(), makeArchive());
    auto zip = ::jxx::NEW<ZipFile>(::jxx::NEW<String>(file.path().u8string()));
    zip->close();

    EXPECT_THROW(zip->entries(), ZipException);
    EXPECT_THROW(zip->getEntry(::jxx::NEW<String>("second.txt")), ZipException);
}

TEST(JxxZipFileTest, CorruptEntryCrcIsRejectedWhenContentIsOpened) {
    auto archive = makeArchive();
    bool changed = false;
    for (::jxx::lang::jint index = 0; index <= archive->length - 4; ++index) {
        const auto b0 = static_cast<std::uint8_t>((*archive)[index]);
        const auto b1 = static_cast<std::uint8_t>((*archive)[index + 1]);
        const auto b2 = static_cast<std::uint8_t>((*archive)[index + 2]);
        const auto b3 = static_cast<std::uint8_t>((*archive)[index + 3]);
        if (b0 == 0x50U && b1 == 0x4bU && b2 == 0x01U && b3 == 0x02U) {
            (*archive)[index + 16] ^= 0x01;
            changed = true;
            break;
        }
    }
    ASSERT_TRUE(changed);

    TemporaryFile file;
    writeNativeFile(file.path(), archive);
    auto zip = ::jxx::NEW<ZipFile>(::jxx::NEW<String>(file.path().u8string()));
    const auto entry = zip->getEntry(::jxx::NEW<String>("folder/first.bin"));
    ASSERT_NE(entry, nullptr);
    EXPECT_THROW(zip->getInputStream(entry), ZipException);
}

TEST(JxxZipFileTest, TruncatedArchiveIsRejected) {
    const auto original = makeArchive();
    ASSERT_GT(original->length, 8);
    auto truncated = ::jxx::NEW<ByteArrayType>(original->length - 8);
    for (::jxx::lang::jint index = 0; index < truncated->length; ++index) {
        (*truncated)[index] = (*original)[index];
    }

    TemporaryFile file;
    writeNativeFile(file.path(), truncated);
    EXPECT_THROW(
        ::jxx::NEW<ZipFile>(::jxx::NEW<String>(file.path().u8string())),
        ZipException);
}

TEST(JxxZipFileTest, Utf8ArchivePathAndEntryNameRoundTrip) {
    TemporaryFile file;
    auto sink = ::jxx::NEW<ByteArrayOutputStream>();
    auto output = ::jxx::NEW<ZipOutputStream>(::jxx::CAST<OutputStream>(sink));
    output->putNextEntry(
        ::jxx::NEW<ZipEntry>(::jxx::NEW<String>(u8"folder/ümlaut.txt")));
    output->write(bytes({1, 2, 3}));
    output->closeEntry();
    output->finish();
    writeNativeFile(file.path(), sink->toByteArray());

    auto zip = ::jxx::NEW<ZipFile>(::jxx::NEW<String>(file.path().u8string()));
    const auto entry = zip->getEntry(::jxx::NEW<String>(u8"folder/ümlaut.txt"));
    ASSERT_NE(entry, nullptr);
    EXPECT_EQ(readAll(zip->getInputStream(entry)),
              (std::vector<std::uint8_t>{1, 2, 3}));
}

} // namespace
