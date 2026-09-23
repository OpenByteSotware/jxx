#include <cstdio>
#include <string>

#include "io/jxx.io.File.h"
#include "io/jxx.io.FileDescriptor.h"
#include "io/jxx.io.FileNotFoundException.h"
#include "io/jxx.io.IOHelper.h"
#include "io/jxx.io.IOException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"
#include "jxx.io.FileOutputStream.h"

namespace
{
    FILE* openOutputFile_(
        const ::jxx::Ptr<::jxx::lang::String>& path,
        ::jxx::lang::jbool append)
    {
#if defined(_WIN32)
        const auto chars = path->toCharArray();
        std::wstring wide;
        wide.reserve(chars->length);
        for (std::uint32_t index = 0; index < chars->length; ++index) {
            wide.push_back(static_cast<wchar_t>((*chars)[index]));
        }
        return ::_wfopen(wide.c_str(), append ? L"ab" : L"wb");
#else
        return std::fopen(path->utf8().c_str(), append ? "ab" : "wb");
#endif
    }
}

namespace jxx::io
{
	FileOutputStream::FileOutputStream(const ::jxx::Ptr<::jxx::lang::String>& n) :FileOutputStream(n, false)
	{
	} FileOutputStream::FileOutputStream(const ::jxx::Ptr<::jxx::lang::String>& n, ::jxx::lang::jbool a) :FileOutputStream(n ? ::jxx::NEW<File>(n) : ::jxx::Ptr<File>(), a)
	{
		if (!n) throw ::jxx::lang::NullPointerException();
	} FileOutputStream::FileOutputStream(const ::jxx::Ptr<File>& f) :FileOutputStream(f, false)
	{
	} FileOutputStream::FileOutputStream(const ::jxx::Ptr<File>& f, ::jxx::lang::jbool a)
	{
		if (!f) throw ::jxx::lang::NullPointerException();
		handle_ = openOutputFile_(f->getPath(), a); if (!handle_)throw FileNotFoundException(f->getPath()); owned_ = true; descriptor_ = ::jxx::NEW<FileDescriptor>(handle_, false);
	} FileOutputStream::FileOutputStream(const ::jxx::Ptr<FileDescriptor>& d) :descriptor_(d)
	{
		if (!d) throw ::jxx::lang::NullPointerException();
		handle_ = d->nativeHandle();
		if (!handle_) throw FileNotFoundException();
	} FileOutputStream::~FileOutputStream()
	{
		try { close(); } catch (...) {}
	} void FileOutputStream::write(::jxx::lang::jint b)
	{
		synchronized([&]
	  {
			   if (!handle_)throw IOException("Stream Closed"); if (std::fputc(b & 0xff, handle_) == EOF)throw IOException();
	  });
	} void FileOutputStream::write(const ::jxx::lang::ByteArray& b, ::jxx::lang::jint o, ::jxx::lang::jint l)
	{
		synchronized([&]
	  {
			   if (!handle_)throw IOException("Stream Closed"); IOHelper::checkBounds(b, o, l); if (std::fwrite(&(*b)[o], 1, static_cast<std::size_t>(l), handle_) != static_cast<std::size_t>(l))throw IOException();
	  });
	} void FileOutputStream::flush()
	{
		synchronized([&]
	  {
			   if (!handle_)throw IOException("Stream Closed"); if (std::fflush(handle_) != 0)throw IOException();
	  });
	} void FileOutputStream::close()
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
	} ::jxx::Ptr<FileDescriptor> FileOutputStream::getFD()const
	{
		return synchronized([&]() -> ::jxx::Ptr<FileDescriptor>
	  {
			   if (!handle_)throw IOException("Stream Closed"); return descriptor_;
	  });
	}
}
