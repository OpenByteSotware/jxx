#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>

#include "io/jxx.io.BufferedInputStream.h"
#include "io/jxx.io.BufferedOutputStream.h"
#include "io/jxx.io.File.h"
#include "io/jxx.io.FileInputStream.h"
#include "io/jxx.io.FileOutputStream.h"
#include "lang/jxx.lang.String.h"
#include "util/zip/jxx.util.zip.ZipEntry.h"
#include "util/zip/jxx.util.zip.ZipFile.h"
#include "util/zip/jxx.util.zip.ZipOutputStream.h"

namespace {
::jxx::Ptr<::jxx::lang::String> text(const std::string& value) {
    return ::jxx::NEW<::jxx::lang::String>(value);
}
}

TEST(ZipCompresserParity, WritesEnumeratesAndReadsZipEntry) {
    const auto root = std::filesystem::temp_directory_path() /
        "jxx_zip_compresser_source_parity";
    std::filesystem::remove_all(root);
    std::filesystem::create_directories(root);
    const auto zipPath = root / "sample.zip";

    auto output = ::jxx::NEW<::jxx::util::zip::ZipOutputStream>(
        ::jxx::NEW<::jxx::io::FileOutputStream>(text(zipPath.string())));
    output->putNextEntry(::jxx::NEW<::jxx::util::zip::ZipEntry>(
        text("folder/data.bin")));
    auto payload = ::jxx::NEW<::jxx::lang::ByteArrayType>(4);
    (*payload)[0] = 1;
    (*payload)[1] = 2;
    (*payload)[2] = 3;
    (*payload)[3] = 4;
    output->write(payload);
    output->closeEntry();
    output->close();

    auto zip = ::jxx::NEW<::jxx::util::zip::ZipFile>(
        ::jxx::NEW<::jxx::io::File>(text(zipPath.string())));
    auto entries = zip->entries();
    ASSERT_TRUE(entries->hasMoreElements());
    const auto entry = entries->nextElement();
    ASSERT_NE(nullptr, entry);
    EXPECT_EQ("folder/data.bin", entry->getName()->utf8());
    EXPECT_FALSE(entry->isDirectory());

    auto input = zip->getInputStream(entry);
    auto readBack = ::jxx::NEW<::jxx::lang::ByteArrayType>(4);
    EXPECT_EQ(4, input->read(readBack));
    EXPECT_EQ(1, static_cast<int>((*readBack)[0]));
    EXPECT_EQ(4, static_cast<int>((*readBack)[3]));
    input->close();
    zip->close();
    std::filesystem::remove_all(root);
}

TEST(ZipCompresserParity, FileDirectoryAndParentOperationsAreAvailable) {
    const auto root = std::filesystem::temp_directory_path() /
        "jxx_zip_compresser_file_parity";
    std::filesystem::remove_all(root);
    auto directory = ::jxx::NEW<::jxx::io::File>(text(root.string()));
    EXPECT_TRUE(directory->mkdirs());
    EXPECT_TRUE(directory->exists());
    EXPECT_TRUE(directory->isDirectory());

    auto child = ::jxx::NEW<::jxx::io::File>(directory, text("child.bin"));
    auto stream = ::jxx::NEW<::jxx::io::FileOutputStream>(child);
    stream->write(7);
    stream->close();
    EXPECT_TRUE(child->isFile());
    ASSERT_NE(nullptr, child->getParentFile());
    EXPECT_EQ(directory->getAbsolutePath()->utf8(),
        child->getParentFile()->getAbsolutePath()->utf8());
    std::filesystem::remove_all(root);
}
