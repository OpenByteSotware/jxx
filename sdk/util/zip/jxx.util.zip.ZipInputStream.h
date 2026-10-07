#pragma once
#include <cstdint>
#include <vector>
#include "util/zip/jxx.util.zip.InflaterInputStream.h"
namespace jxx::nio::charset
{
	class Charset;
}
namespace jxx::util::zip
{
	class ZipEntry;
	class ZipInputStream final : public ::jxx::lang::ClassBase<ZipInputStream, InflaterInputStream>
	{
	public:
		using JxxSuper = InflaterInputStream; using Super = ::jxx::lang::ClassBase<ZipInputStream, JxxSuper>; using JxxClassInfoMarker = typename Super::JxxClassInfoMarker;
		explicit ZipInputStream(const ::jxx::Ptr<::jxx::io::InputStream>& input);
		ZipInputStream(const ::jxx::Ptr<::jxx::io::InputStream>& input, const ::jxx::Ptr<::jxx::nio::charset::Charset>& charset); ~ZipInputStream()override;
		::jxx::Ptr<ZipEntry> getNextEntry(); void closeEntry();
		::jxx::lang::jint read()override; 
		::jxx::lang::jint read(const ::jxx::lang::ByteArray& buffer)override; 
		::jxx::lang::jint read(const ::jxx::lang::ByteArray& buffer, ::jxx::lang::jint offset, ::jxx::lang::jint length)override; ::jxx::lang::jlong skip(::jxx::lang::jlong count)override; ::jxx::lang::jint available()override; void close()override;
	protected:
		void fill() override;
	private:
		::jxx::lang::jint raw_(); void exact_(std::uint8_t*, std::size_t);
		std::uint16_t u16_(); std::uint32_t u32_(); void preserveRemaining_();
		void verify_();
		std::vector<std::uint8_t> pending_; std::size_t pendingPos_ = 0;
		::jxx::Ptr<ZipEntry> entry_;
		std::uint32_t crc_ = 0, readCount_ = 0, expectedCompressed_ = 0;
		bool descriptor_ = false, entryEof_ = true, closed_ = false;
	};
}
