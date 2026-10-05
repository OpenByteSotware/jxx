#include <gtest/gtest.h>
#include <string>
#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "util/zip/jxx.util.zip.DeflaterOutputStream.h"
#include "util/zip/jxx.util.zip.InflaterInputStream.h"
namespace
{
	::jxx::lang::ByteArray bytes(const std::string& s)
	{
		auto a = ::jxx::NEW<::jxx::lang::ByteArrayType>(static_cast<::jxx::lang::jint>(s.size()));
		for (::jxx::lang::jint i = 0; i < a->length; ++i)(*a)[i] = static_cast<::jxx::lang::jbyte>(s[static_cast<std::size_t>(i)]); return a;
	}
	TEST(JxxDeflaterInflaterStreamTest, RoundTrip)
	{
		auto source = bytes("stream parity stream parity stream parity");
		auto sink = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
		auto compressed = ::jxx::NEW<::jxx::util::zip::DeflaterOutputStream>(sink);
		compressed->write(source); compressed->finish(); auto packed = sink->toByteArray();
		auto input = ::jxx::NEW<::jxx::io::ByteArrayInputStream>(packed);
		auto inflated = ::jxx::NEW<::jxx::util::zip::InflaterInputStream>(input);
		auto output = ::jxx::NEW<::jxx::lang::ByteArrayType>(source->length);
		EXPECT_EQ(source->length, inflated->read(output)); for (::jxx::lang::jint i = 0; i < source->length; ++i)EXPECT_EQ((*source)[i], (*output)[i]); EXPECT_EQ(-1, inflated->read());
	}
	TEST(JxxDeflaterInflaterStreamTest, SkipConsumesUncompressedBytes)
	{
		auto source = bytes("abcdef"); auto sink = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
		auto compressed = ::jxx::NEW<::jxx::util::zip::DeflaterOutputStream>(sink);
		compressed->write(source); compressed->finish(); auto input = ::jxx::NEW<::jxx::io::ByteArrayInputStream>(sink->toByteArray()); auto inflated = ::jxx::NEW<::jxx::util::zip::InflaterInputStream>(input); EXPECT_EQ(3, inflated->skip(3)); EXPECT_EQ('d', inflated->read());
	}
	TEST(JxxDeflaterInflaterStreamTest, SyncFlushProducesReadablePrefix)
	{
		auto first = bytes("first"); auto sink = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
		auto compressed = ::jxx::NEW<::jxx::util::zip::DeflaterOutputStream>(sink, true); compressed->write(first); compressed->flush(); EXPECT_GT(sink->size(), 0);
	}
} // namespace
