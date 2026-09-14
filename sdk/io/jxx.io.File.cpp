#include "io/jxx.io.File.h"
#include <chrono>
#include <filesystem>
#include <fstream>
#include <cstdlib>
#include <functional>
#include <mutex>
#include <vector>
#include <string>
#include "io/jxx.io.IOException.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"
namespace jxx::io
{
	namespace fs = std::filesystem;

	namespace {
		std::mutex& deleteOnExitMutex()
		{
			static std::mutex value;
			return value;
		}

		std::vector<fs::path>& deleteOnExitPaths()
		{
			static auto* value = new std::vector<fs::path>();
			return *value;
		}

		void deleteRegisteredPaths() noexcept
		{
			std::vector<fs::path> paths;
			{
				std::lock_guard<std::mutex> guard(deleteOnExitMutex());
				paths.swap(deleteOnExitPaths());
			}
			for (auto iterator = paths.rbegin();
				 iterator != paths.rend(); ++iterator) {
				std::error_code error;
				fs::remove(*iterator, error);
			}
		}

		void registerDeleteOnExit(const fs::path& path)
		{
			static const bool registered = [] {
				std::atexit(deleteRegisteredPaths);
				return true;
			}();
			(void)registered;
			std::lock_guard<std::mutex> guard(deleteOnExitMutex());
			deleteOnExitPaths().push_back(path);
		}
	} // namespace
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
	} File::File(
		const ::jxx::Ptr<::jxx::lang::String>& parent,
		const ::jxx::Ptr<::jxx::lang::String>& child)
		: File([&]() -> ::jxx::Ptr<::jxx::lang::String> {
			if (child == nullptr) {
				throw ::jxx::lang::NullPointerException();
			}
			if (parent == nullptr) {
				return child;
			}
			return ::jxx::NEW<::jxx::lang::String>(
				(native(parent) / native(child)).u8string());
		}())
	{
	} File::File(
		const ::jxx::Ptr<File>& parent,
		const ::jxx::Ptr<::jxx::lang::String>& child)
		: File(
			parent == nullptr ? nullptr : parent->getPath(),
			child)
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
		registerDeleteOnExit(native(path_));
	} ::jxx::lang::jbool File::mkdir()
	{
		std::error_code e; return fs::create_directory(native(path_), e);
	} ::jxx::lang::jbool File::mkdirs()
	{
		std::error_code e; return fs::create_directories(native(path_), e);
	} ::jxx::lang::jbool File::renameTo(const ::jxx::Ptr<File>& d)
	{
		if (!d)return false; std::error_code e; fs::rename(native(path_), native(d->path_), e); return !e;
	} ::jxx::lang::jbool File::setLastModified(::jxx::lang::jlong time)
	{
		if (time < 0) {
			throw ::jxx::lang::IllegalArgumentException();
		}

		std::error_code error;
		const auto systemTime =
			std::chrono::system_clock::time_point(
				std::chrono::milliseconds(time));
		const auto fileTime =
			fs::file_time_type::clock::now() +
			(systemTime - std::chrono::system_clock::now());
		fs::last_write_time(native(path_), fileTime, error);
		return !error;
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
