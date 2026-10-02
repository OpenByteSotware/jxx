#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include "io/jxx.io.File.h"
#include "io/jxx.io.FileOutputStream.h"
#include "io/jxx.io.InputStream.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"
#include "util/jxx.util.Enumeration.h"
#include "util/zip/jxx.util.zip.ZipEntry.h"
#include "util/zip/jxx.util.zip.ZipException.h"
#include "util/zip/jxx.util.zip.ZipFile.h"
#include "util/zip/jxx.util.zip.ZipOutputStream.h"
namespace
{
	TEST(ZipApplicationParity, WritesEnumeratesAndReadsEntries)
	{
		const auto path = std::filesystem::temp_directory_path() / "jxx_zip_app_parity.zip"; std::filesystem::remove(path);
		const auto file = ::jxx::NEW<::jxx::io::File>(::jxx::NEW<::jxx::lang::String>(path.u8string()));
		auto output = ::jxx::NEW<::jxx::util::zip::ZipOutputStream>(::jxx::CAST<::jxx::io::OutputStream>(::jxx::NEW<::jxx::io::FileOutputStream>(file)));
		const auto entry = ::jxx::NEW<::jxx::util::zip::ZipEntry>(::jxx::NEW<::jxx::lang::String>("folder/data.txt")); output->putNextEntry(entry);
		const auto data = ::jxx::NEW<::jxx::lang::ByteArrayType>(3); (*data)[0] = 'a'; (*data)[1] = 'b';
		(*data)[2] = 'c'; output->write(data); output->closeEntry(); output->close();
		auto input = ::jxx::NEW<::jxx::util::zip::ZipFile>(file); EXPECT_EQ(1, input->size()); 
		auto entries = input->entries(); ASSERT_TRUE(entries->hasMoreElements()); 
		auto readEntry = entries->nextElement(); 
		EXPECT_EQ("folder/data.txt", readEntry->getName()->utf8()); 
		auto stream = input->getInputStream(readEntry); auto read = ::jxx::NEW<::jxx::lang::ByteArrayType>(3); EXPECT_EQ(3, stream->read(read));
		EXPECT_EQ('a', (*read)[0]); EXPECT_EQ('b', (*read)[1]); EXPECT_EQ('c', (*read)[2]); stream->close(); input->close(); std::filesystem::remove(path);
	}
	TEST(ZipApplicationParity, SupportsEmptyAndUnicodeNamedEntries)
	{
		const auto path = std::filesystem::temp_directory_path() / "jxx_zip_unicode.zip"; std::filesystem::remove(path);
		const auto file = ::jxx::NEW<::jxx::io::File>(::jxx::NEW<::jxx::lang::String>(path.u8string())); 
		auto output = ::jxx::NEW<::jxx::util::zip::ZipOutputStream>(::jxx::CAST<::jxx::io::OutputStream>(::jxx::NEW<::jxx::io::FileOutputStream>(file)));
		output->putNextEntry(::jxx::NEW<::jxx::util::zip::ZipEntry>(::jxx::NEW<::jxx::lang::String>(u8"資料.txt"))); output->closeEntry(); 
		output->close(); auto input = ::jxx::NEW<::jxx::util::zip::ZipFile>(file); EXPECT_NE(nullptr, input->getEntry(::jxx::NEW<::jxx::lang::String>(u8"資料.txt"))); input->close(); std::filesystem::remove(path);
	}
	TEST(ZipApplicationParity, MalformedArchiveThrowsZipException)
	{
		const auto path = std::filesystem::temp_directory_path() / "jxx_bad.zip"; std::ofstream(path, std::ios::binary) << "bad";
		const auto file = ::jxx::NEW<::jxx::io::File>(::jxx::NEW<::jxx::lang::String>(path.u8string()));
		EXPECT_THROW(
			(void)::jxx::NEW<::jxx::util::zip::ZipFile>(file),
			::jxx::util::zip::ZipException);
		std::filesystem::remove(path);
	}
}
