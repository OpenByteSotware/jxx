#include "io/jxx.io.File.h"
#include <chrono>
#include <filesystem>
#include <fstream>
#include <functional>
#include <string>
#include "io/jxx.io.IOException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"
namespace jxx::io
{
	namespace fs = std::filesystem;
#ifdef _WIN32
	const ::jxx::lang::jchar File::separatorChar = u'\\'; const ::jxx::lang::jchar File::pathSeparatorChar = u';';
#else
	const ::jxx::lang::jchar File::separatorChar = u'/'; const ::jxx::lang::jchar File::pathSeparatorChar = u':';
#endif
	::jxx::Ptr<::jxx::lang::String> File::separator = ::jxx::NEW<::jxx::lang::String>(std::u16string(1, separatorChar)); ::jxx::Ptr<::jxx::lang::String> File::pathSeparator = ::jxx::NEW<::jxx::lang::String>(std::u16string(1, pathSeparatorChar));
	static fs::path native(const ::jxx::Ptr<::jxx::lang::String>& s)
	{
		return fs::u8path(s->utf8());
	} File::File(const ::jxx::Ptr<::jxx::lang::String>& p) :path_(p)
	{
		if (!p)throw ::jxx::lang::NullPointerException();
	} File::File(const ::jxx::Ptr<::jxx::lang::String>& p, const ::jxx::Ptr<::jxx::lang::String>& c) :File(::jxx::NEW<::jxx::lang::String>((native(p) / native(c)).u8string()))
	{
	} File::File(const ::jxx::Ptr<File>& p, const ::jxx::Ptr<::jxx::lang::String>& c) :File(p ? p->getPath() : nullptr, c)
	{
	}
	::jxx::Ptr<::jxx::lang::String> File::getName()const
	{
		return ::jxx::NEW<::jxx::lang::String>(native(path_).filename().u8string());
	} ::jxx::Ptr<::jxx::lang::String> File::getParent()const
	{
		auto p = native(path_).parent_path(); return p.empty() ? nullptr : jxx::NEW<::jxx::lang::String>(p.u8string());
	} ::jxx::Ptr<File> File::getParentFile()const
	{
		auto p = getParent(); return p ? ::jxx::NEW<File>(p) : nullptr;
	} ::jxx::Ptr<::jxx::lang::String> File::getPath()const
	{
		return path_;
	} ::jxx::lang::jbool File::isAbsolute()const
	{
		return native(path_).is_absolute();
	} ::jxx::Ptr<::jxx::lang::String> File::getAbsolutePath()const
	{
		return ::jxx::NEW<::jxx::lang::String>(fs::absolute(native(path_)).u8string());
	} ::jxx::Ptr<File> File::getAbsoluteFile()const
	{
		return ::jxx::NEW<File>(getAbsolutePath());
	} ::jxx::Ptr<::jxx::lang::String> File::getCanonicalPath()const
	{
		try {
			return ::jxx::NEW<::jxx::lang::String>(fs::weakly_canonical(native(path_)).u8string());
		}
		catch (const fs::filesystem_error& e) {
			throw IOException(e.what());
		}
	} ::jxx::Ptr<File> File::getCanonicalFile()const
	{
		return ::jxx::NEW<File>(getCanonicalPath());
	}
	::jxx::lang::jbool File::canRead()const
	{
		std::ifstream f(native(path_), std::ios::binary); return f.good();
	} ::jxx::lang::jbool File::canWrite()const
	{
		auto p = native(path_); std::error_code e; auto perms = fs::status(p, e).permissions(); return !e && (perms & fs::perms::owner_write) != fs::perms::none;
	} ::jxx::lang::jbool File::canExecute()const
	{
		std::error_code e; auto p = fs::status(native(path_), e).permissions(); return !e && (p & (fs::perms::owner_exec | fs::perms::group_exec | fs::perms::others_exec)) != fs::perms::none;
	} ::jxx::lang::jbool File::exists()const
	{
		std::error_code e; return fs::exists(native(path_), e);
	} ::jxx::lang::jbool File::isDirectory()const
	{
		std::error_code e; return fs::is_directory(native(path_), e);
	} ::jxx::lang::jbool File::isFile()const
	{
		std::error_code e; return fs::is_regular_file(native(path_), e);
	} ::jxx::lang::jbool File::isHidden()const
	{
		auto n = getName()->utf8(); return !n.empty() && n[0] == '.';
	} ::jxx::lang::jlong File::lastModified()const
	{
		std::error_code e; auto t = fs::last_write_time(native(path_), e); if (e)return 0; return std::chrono::duration_cast<std::chrono::milliseconds>(t.time_since_epoch()).count();
	} ::jxx::lang::jlong File::length()const
	{
		std::error_code e; auto n = fs::file_size(native(path_), e); return e ? 0 : static_cast<::jxx::lang::jlong>(n);
	} ::jxx::lang::jbool File::createNewFile()
	{
		if (exists())return false; std::ofstream f(native(path_), std::ios::binary); return f.good();
	} ::jxx::lang::jbool File::delete_()
	{
		std::error_code e; return fs::remove(native(path_), e);
	} void File::deleteOnExit()
	{
	} ::jxx::lang::jbool File::mkdir()
	{
		std::error_code e; return fs::create_directory(native(path_), e);
	} ::jxx::lang::jbool File::mkdirs()
	{
		std::error_code e; return fs::create_directories(native(path_), e);
	} ::jxx::lang::jbool File::renameTo(const ::jxx::Ptr<File>& d)
	{
		if (!d)return false; std::error_code e; fs::rename(native(path_), native(d->path_), e); return !e;
	} ::jxx::lang::jbool File::setLastModified(::jxx::lang::jlong)
	{
		return false;
	} ::jxx::lang::jbool File::setReadOnly()
	{
		std::error_code e; fs::permissions(native(path_), fs::perms::owner_write | fs::perms::group_write | fs::perms::others_write, fs::perm_options::remove, e); return !e;
	} ::jxx::lang::jbool File::setWritable(::jxx::lang::jbool v, ::jxx::lang::jbool)
	{
		std::error_code e; fs::permissions(native(path_), fs::perms::owner_write, v ? fs::perm_options::add : fs::perm_options::remove, e); return !e;
	} ::jxx::lang::jbool File::setReadable(::jxx::lang::jbool v, ::jxx::lang::jbool)
	{
		std::error_code e; fs::permissions(native(path_), fs::perms::owner_read, v ? fs::perm_options::add : fs::perm_options::remove, e); return !e;
	} ::jxx::lang::jbool File::setExecutable(::jxx::lang::jbool v, ::jxx::lang::jbool)
	{
		std::error_code e; fs::permissions(native(path_), fs::perms::owner_exec, v ? fs::perm_options::add : fs::perm_options::remove, e); return !e;
	} ::jxx::lang::jlong File::getTotalSpace()const
	{
		std::error_code e; return static_cast<::jxx::lang::jlong>(fs::space(native(path_), e).capacity);
	} ::jxx::lang::jlong File::getFreeSpace()const
	{
		std::error_code e; return static_cast<::jxx::lang::jlong>(fs::space(native(path_), e).free);
	} ::jxx::lang::jlong File::getUsableSpace()const
	{
		std::error_code e; return static_cast<::jxx::lang::jlong>(fs::space(native(path_), e).available);
	} ::jxx::lang::jint File::compareTo(const ::jxx::Ptr<File>& o)const
	{
		return path_->compareTo(o->path_);
	} ::jxx::lang::jbool File::equals(const ::jxx::Ptr<::jxx::lang::Object>& o)const
	{
		auto f = ::jxx::CAST<File>(o); return f && path_->equals(f->path_);
	} ::jxx::lang::jint File::hashCode()const
	{
		return path_->hashCode();
	} ::jxx::Ptr<::jxx::lang::String> File::toString()const
	{
		return path_;
	}
}
