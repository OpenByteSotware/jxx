#include <gtest/gtest.h>
#include <string>
#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "lang/jxx.lang.String.h"
#include "util/zip/jxx.util.zip.ZipEntry.h"
#include "util/zip/jxx.util.zip.ZipInputStream.h"
#include "util/zip/jxx.util.zip.ZipOutputStream.h"
namespace
{
	::jxx::lang::ByteArray bytes(const std::string& s)
	{
		auto a = ::jxx::NEW<::jxx::lang::ByteArrayType>(static_cast<::jxx::lang::jint>(s.size())); for (::jxx::lang::jint i = 0; i < a->length; ++i)(*a)[i] = static_cast<::jxx::lang::jbyte>(s[static_cast<std::size_t>(i)]); return a;
	}
	TEST(JxxZipStreamingTest, DeflatedDataDescriptorRoundTrip)
	{
		auto sink = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>(); auto zout = ::jxx::NEW<::jxx::util::zip::ZipOutputStream>(sink); auto e = ::jxx::NEW<::jxx::util::zip::ZipEntry>(::jxx::NEW<::jxx::lang::String>("a.txt")); auto data = bytes("streamed zip entry"); zout->putNextEntry(e); zout->write(data); zout->closeEntry(); zout->finish(); auto zin = ::jxx::NEW<::jxx::util::zip::ZipInputStream>(::jxx::NEW<::jxx::io::ByteArrayInputStream>(sink->toByteArray())); auto readEntry = zin->getNextEntry(); ASSERT_NE(nullptr, readEntry); EXPECT_TRUE(readEntry->getName()->equals(::jxx::NEW<::jxx::lang::String>("a.txt"))); auto out = ::jxx::NEW<::jxx::lang::ByteArrayType>(data->length); EXPECT_EQ(data->length, zin->read(out)); for (::jxx::lang::jint i = 0; i < data->length; ++i)EXPECT_EQ((*data)[i], (*out)[i]); EXPECT_EQ(-1, zin->read()); EXPECT_EQ(nullptr, zin->getNextEntry());
	}
	TEST(JxxZipStreamingTest, MultipleEntriesRoundTrip)
	{
		auto sink = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>(); auto z = ::jxx::NEW<::jxx::util::zip::ZipOutputStream>(sink); for (const char* n : { "one","two" }) {
			z->putNextEntry(::jxx::NEW<::jxx::util::zip::ZipEntry>(::jxx::NEW<::jxx::lang::String>(n))); 
			z->write(bytes(n)); z->closeEntry();
		}z->finish(); auto in = 
			::jxx::NEW<::jxx::util::zip::ZipInputStream>(::jxx::NEW<::jxx::io::ByteArrayInputStream>(sink->toByteArray())); 
		EXPECT_NE(nullptr, in->getNextEntry()); in->closeEntry();
		EXPECT_NE(nullptr, in->getNextEntry()); 
		in->closeEntry();
		EXPECT_EQ(nullptr, in->getNextEntry());
	}
}
