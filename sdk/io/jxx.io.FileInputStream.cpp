#include <algorithm>
#include <cerrno>
#include <cstdio>
#include <limits>
#include <string>

#include "io/jxx.io.File.h"
#include "io/jxx.io.FileDescriptor.h"
#include "io/jxx.io.FileNotFoundException.h"
#include "io/jxx.io.IOHelper.h"
#include "io/jxx.io.IOException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"
#include "jxx.io.FileInputStream.h"

namespace
{
    long long tellFile_(FILE* handle)
    {
#if defined(_WIN32)
        return ::_ftelli64(handle);
#else
        return static_cast<long long>(::ftello(handle));
#endif
    }

    int seekFile_(FILE* handle, long long offset, int origin)
    {
#if defined(_WIN32)
        return ::_fseeki64(handle, offset, origin);
#else
        return ::fseeko(handle, static_cast<off_t>(offset), origin);
#endif
    }

    FILE* openInputFile_(const ::jxx::Ptr<::jxx::lang::String>& path)
    {
#if defined(_WIN32)
        const auto chars = path->toCharArray();
        std::wstring wide;
        wide.reserve(chars->length);
        for (std::uint32_t index = 0; index < chars->length; ++index) {
            wide.push_back(static_cast<wchar_t>((*chars)[index]));
        }
        return ::_wfopen(wide.c_str(), L"rb");
#else
        return std::fopen(path->utf8().c_str(), "rb");
#endif
    }
}

namespace jxx::io
{
	FileInputStream::FileInputStream(const ::jxx::Ptr<::jxx::lang::String>& n)
	{
		if (!n) throw ::jxx::lang::NullPointerException();
		const auto file = ::jxx::NEW<File>(n);
		if (file->isDirectory()) throw FileNotFoundException(file->getPath());
		handle_ = openInputFile_(file->getPath());
		if (!handle_) throw FileNotFoundException(file->getPath());
		owned_ = true;
		descriptor_ = ::jxx::NEW<FileDescriptor>(handle_, false);
	}

	FileInputStream::FileInputStream(const ::jxx::Ptr<File>& f)
	{
		if (!f) throw ::jxx::lang::NullPointerException();
		if (f->isDirectory()) throw FileNotFoundException(f->getPath());
		handle_ = openInputFile_(f->getPath());
		if (!handle_) throw FileNotFoundException(f->getPath());
		owned_ = true;
		descriptor_ = ::jxx::NEW<FileDescriptor>(handle_, false);
	} FileInputStream::FileInputStream(const ::jxx::Ptr<FileDescriptor>& d) :descriptor_(d)
	{
		if (!d)throw ::jxx::lang::NullPointerException(); handle_ = d->nativeHandle(); if (!handle_)throw FileNotFoundException();
	} FileInputStream::~FileInputStream()
	{
		try { close(); } catch (...) {}
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
			   if (!handle_)throw IOException("Stream Closed"); if (n <= 0)return 0; auto p = tellFile_(handle_); if (p < 0)throw IOException(); const auto step = static_cast<long long>(n); if (seekFile_(handle_, step, SEEK_CUR) != 0)throw IOException(); auto q = tellFile_(handle_); if (q < 0)throw IOException(); return static_cast<::jxx::lang::jlong>(q - p);
	  });
	} ::jxx::lang::jint FileInputStream::available()
	{
		return synchronized([&]() -> ::jxx::lang::jint
	  {
			   if (!handle_)throw IOException("Stream Closed"); auto p = tellFile_(handle_); if (p < 0)throw IOException(); if (seekFile_(handle_, 0, SEEK_END) != 0)throw IOException(); auto e = tellFile_(handle_); if (e < 0) {
				   seekFile_(handle_, p, SEEK_SET); throw IOException();
			   }if (seekFile_(handle_, p, SEEK_SET) != 0)throw IOException(); const auto remaining = std::max<long long>(0, e - p); const auto maximum = std::numeric_limits<::jxx::lang::jint>::max(); return remaining > maximum ? maximum : static_cast<::jxx::lang::jint>(remaining);
	  });
	} void FileInputStream::close()
	{
		synchronized([&]
	  {
			   if (!handle_) return;
			   auto* const handle = handle_;
			   const auto owned = owned_;
			   handle_ = nullptr;
			   owned_ = false;
			   if (owned && descriptor_) descriptor_->invalidate(handle);
			   if (owned && std::fclose(handle) != 0) throw IOException();
	  });
	} ::jxx::Ptr<FileDescriptor> FileInputStream::getFD()const
	{
		return synchronized([&]() -> ::jxx::Ptr<FileDescriptor>
	  {
			   if (!handle_)throw IOException("Stream Closed"); return descriptor_;
	  });
	}
}
