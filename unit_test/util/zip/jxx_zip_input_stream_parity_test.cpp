#include <gtest/gtest.h>
#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.EOFException.h"
#include "lang/jxx.lang.buildin_array.h"
#include "util/zip/jxx.util.zip.ZipInputStream.h"
namespace
{
	TEST(JxxZipInputStreamParityTest, CleanPhysicalEofReturnsNullEntry)
	{
		auto empty = ::jxx::NEW<::jxx::lang::ByteArrayType>(0);
		auto input = ::jxx::NEW<::jxx::util::zip::ZipInputStream>(::jxx::NEW<::jxx::io::ByteArrayInputStream>(empty)); 
		EXPECT_EQ(nullptr, input->getNextEntry());
	}
	TEST(JxxZipInputStreamParityTest, PartialSignatureIsTruncatedInput)
	{
		auto bytes = ::jxx::NEW<::jxx::lang::ByteArrayType>(2); (*bytes)[0] = 0x50; 
		(*bytes)[1] = 0x4b; 
		auto input = ::jxx::NEW<::jxx::util::zip::ZipInputStream>(::jxx::NEW<::jxx::io::ByteArrayInputStream>(bytes));
		EXPECT_THROW(input->getNextEntry(), ::jxx::io::EOFException);
	}
}
