#include <cstdio>
#include <algorithm>
#include <cerrno>
#include <limits>
#include "io/jxx.io.File.h"
#include "io/jxx.io.FileDescriptor.h"
#include "io/jxx.io.FileNotFoundException.h"
#include "io/jxx.io.IOHelper.h"
#include "io/jxx.io.IOException.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "io/jxx.io.FileInputStream.h"

namespace jxx::io
{
	FileInputStream::FileInputStream(const ::jxx::Ptr<::jxx::lang::String>& n) :FileInputStream(n ? ::jxx::NEW<File>(n) : ::jxx::Ptr<File>())
	{
	} FileInputStream::FileInputStream(const ::jxx::Ptr<File>& f)
	{
		handle_ = std::fopen(f->getPath()->utf8().c_str(), "rb"); if (!handle_)throw FileNotFoundException(f->getPath()); owned_ = true; descriptor_ = ::jxx::NEW<FileDescriptor>(handle_, false);
	} FileInputStream::FileInputStream(const ::jxx::Ptr<FileDescriptor>& d) :descriptor_(d)
	{
		if (!d)throw ::jxx::lang::NullPointerException(); handle_ = d->nativeHandle(); if (!handle_)throw FileNotFoundException();
	} FileInputStream::~FileInputStream()
	{
		close();
	} ::jxx::lang::jint FileInputStream::read()
	{
		return synchronized([&]() -> ::jxx::lang::jint
	  {
			   if (!handle_)throw IOException("Stream Closed"); auto c = std::fgetc(handle_); if (c != EOF)return c; if (std::ferror(handle_) != 0)throw IOException(); return -1;
	  });
	} ::jxx::lang::jint FileInputStream::read(const ::jxx::lang::ByteArray& b, ::jxx::lang::jint o, ::jxx::lang::jint l)
	{
		return synchronized([&]() -> ::jxx::lang::jint
	  {
			   if (!handle_)throw IOException("Stream Closed"); IOHelper::checkBounds(b, o, l); if (l == 0)return 0; auto n = std::fread(&(*b)[o], 1, static_cast<std::size_t>(l), handle_); if (n > 0)return static_cast<::jxx::lang::jint>(n); if (std::ferror(handle_) != 0)throw IOException(); return -1;
	  });
	} ::jxx::lang::jlong FileInputStream::skip(::jxx::lang::jlong n)
	{
		return synchronized([&]() -> ::jxx::lang::jlong
	  {
			   if (!handle_)throw IOException("Stream Closed"); if (n <= 0)return 0; auto p = std::ftell(handle_); if (p < 0)throw IOException(); const auto step = std::min<::jxx::lang::jlong>(n, static_cast<::jxx::lang::jlong>(std::numeric_limits<long>::max())); if (std::fseek(handle_, static_cast<long>(step), SEEK_CUR) != 0)throw IOException(); auto q = std::ftell(handle_); if (q < 0)throw IOException(); return static_cast<::jxx::lang::jlong>(q - p);
	  });
	} ::jxx::lang::jint FileInputStream::available()
	{
		return synchronized([&]() -> ::jxx::lang::jint
	  {
			   if (!handle_)throw IOException("Stream Closed"); auto p = std::ftell(handle_); if (p < 0)throw IOException(); if (std::fseek(handle_, 0, SEEK_END) != 0)throw IOException(); auto e = std::ftell(handle_); if (e < 0) {
				   std::fseek(handle_, p, SEEK_SET); throw IOException();
			   }if (std::fseek(handle_, p, SEEK_SET) != 0)throw IOException(); const auto remaining = std::max<long>(0, e - p); const auto maximum = std::numeric_limits<::jxx::lang::jint>::max(); return remaining > maximum ? maximum : static_cast<::jxx::lang::jint>(remaining);
	  });
	} void FileInputStream::close()
	{
		synchronized([&]
	  {
			   if (owned_ && handle_)std::fclose(handle_); handle_ = nullptr; owned_ = false;
	  });
	} ::jxx::Ptr<FileDescriptor> FileInputStream::getFD()const
	{
		return synchronized([&]() -> ::jxx::Ptr<FileDescriptor>
	  {
			   if (!handle_)throw IOException("Stream Closed"); return descriptor_;
	  });
	}
}
