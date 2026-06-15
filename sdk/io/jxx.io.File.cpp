
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <mutex>
#include <stdexcept>
#include <string>
#include <system_error>
#include <vector>
#include "lang/jxx.lang.String.h"
#include "jxx.io.ObjectOutputStream.h"
#include "jxx.io.ObjectInputStream.h"
#include "jxx.io.File.h"

namespace fs = std::filesystem;

namespace
{
    void throwIAE_(const char* msg)
    {
        throw std::invalid_argument(msg);
    }

    inline std::string toUtf8_(const jxx::Ptr<jxx::lang::String>& s)
    {
        return s ? s->utf8() : std::string{};
    }

    inline std::string joinPath_(const std::string& parent, const std::string& child)
    {
        if (parent.empty())
            return child;

        fs::path p(parent);
        p /= child;
        return p.generic_string();
    }

    inline jxx::Ptr<jxx::lang::String> toStringPtr_(const std::string& s)
    {
        return std::make_shared<jxx::lang::String>(s);
    }

    inline std::mutex& deleteOnExitMutex_()
    {
        static std::mutex m;
        return m;
    }

    inline std::vector<std::string>& deleteOnExitList_()
    {
        static std::vector<std::string> files;
        return files;
    }

    inline void runDeleteOnExit_()
    {
        std::lock_guard<std::mutex> lock(deleteOnExitMutex_());
        for (const auto& p : deleteOnExitList_())
        {
            std::error_code ec;
            fs::remove(p, ec);
        }
        deleteOnExitList_().clear();
    }

    inline void ensureDeleteOnExitRegistered_()
    {
        static const bool registered = [] {
            std::atexit(runDeleteOnExit_);
            return true;
        }();
        (void)registered;
    }
}

namespace jxx::io
{
    File::File(jxx::Ptr<jxx::lang::String> pathname)
        : path_(toUtf8_(pathname))
    {
        if (!pathname)
            throwIAE_("null pathname");
    }

    File::File(jxx::Ptr<jxx::lang::String> parent,
               jxx::Ptr<jxx::lang::String> child)
    {
        if (!child)
            throwIAE_("null child");

        path_ = joinPath_(toUtf8_(parent), toUtf8_(child));
    }

    File::File(jxx::Ptr<File> parent,
               jxx::Ptr<jxx::lang::String> child)
    {
        if (!child)
            throwIAE_("null child");

        path_ = joinPath_(parent ? parent->path_ : std::string{}, toUtf8_(child));
    }

    jxx::Ptr<jxx::lang::String> File::getName() const
    {
        return toStringPtr_(fs::path(path_).filename().generic_string());
    }

    jxx::Ptr<jxx::lang::String> File::getParent() const
    {
        const auto p = fs::path(path_).parent_path().generic_string();
        return p.empty() ? nullptr : toStringPtr_(p);
    }

    jxx::Ptr<File> File::getParentFile() const
    {
        auto p = getParent();
        return p ? std::make_shared<File>(p) : nullptr;
    }

    jxx::Ptr<jxx::lang::String> File::getPath() const
    {
        return toStringPtr_(path_);
    }

    jxx::lang::jbool File::isAbsolute() const
    {
        return fs::path(path_).is_absolute();
    }

    jxx::Ptr<jxx::lang::String> File::getAbsolutePath() const
    {
        std::error_code ec;
        const auto p = fs::absolute(fs::path(path_), ec);
        return toStringPtr_(ec ? fs::path(path_).generic_string() : p.generic_string());
    }

    jxx::Ptr<File> File::getAbsoluteFile() const
    {
        return std::make_shared<File>(getAbsolutePath());
    }

    jxx::Ptr<jxx::lang::String> File::getCanonicalPath() const
    {
        std::error_code ec;
        const auto p = fs::weakly_canonical(fs::path(path_), ec);
        if (!ec)
            return toStringPtr_(p.generic_string());

        const auto a = fs::absolute(fs::path(path_), ec);
        return toStringPtr_(ec ? path_ : a.generic_string());
    }

    jxx::Ptr<File> File::getCanonicalFile() const
    {
        return std::make_shared<File>(getCanonicalPath());
    }

    jxx::lang::jbool File::canRead() const
    {
        std::ifstream in(path_, std::ios::binary);
        return in.good();
    }

    jxx::lang::jbool File::canWrite() const
    {
        std::ofstream out(path_, std::ios::app | std::ios::binary);
        return out.good();
    }

    jxx::lang::jbool File::canExecute() const
    {
    #if defined(_WIN32)
        return exists();
    #else
        std::error_code ec;
        auto st = fs::status(path_, ec);
        if (ec || !fs::exists(st))
            return false;

        auto perms = st.permissions();
        return ((perms & fs::perms::owner_exec) != fs::perms::none)
            || ((perms & fs::perms::group_exec) != fs::perms::none)
            || ((perms & fs::perms::others_exec) != fs::perms::none);
    #endif
    }

    jxx::lang::jbool File::exists() const
    {
        std::error_code ec;
        return fs::exists(path_, ec) && !ec;
    }

    jxx::lang::jbool File::isDirectory() const
    {
        std::error_code ec;
        return fs::is_directory(path_, ec) && !ec;
    }

    jxx::lang::jbool File::isFile() const
    {
        std::error_code ec;
        return fs::is_regular_file(path_, ec) && !ec;
    }

    jxx::lang::jbool File::isHidden() const
    {
        const auto name = fs::path(path_).filename().generic_string();
        return !name.empty() && name[0] == '.';
    }

    jxx::lang::jlong File::lastModified() const
    {
        std::error_code ec;
        const auto ft = fs::last_write_time(path_, ec);
        if (ec)
            return 0;

        const auto sctp = std::chrono::time_point_cast<std::chrono::milliseconds>(
            ft - fs::file_time_type::clock::now() + std::chrono::system_clock::now());

        return static_cast<jxx::lang::jlong>(sctp.time_since_epoch().count());
    }

    jxx::lang::jlong File::length() const
    {
        std::error_code ec;
        const auto sz = fs::file_size(path_, ec);
        return ec ? 0 : static_cast<jxx::lang::jlong>(sz);
    }

    jxx::lang::jbool File::createNewFile()
    {
        if (exists())
            return false;

        std::ofstream out(path_, std::ios::binary);
        return out.good();
    }

    jxx::lang::jbool File::delete_()
    {
        std::error_code ec;
        return fs::remove(path_, ec) && !ec;
    }

    void File::deleteOnExit()
    {
        ensureDeleteOnExitRegistered_();
        std::lock_guard<std::mutex> lock(deleteOnExitMutex_());
        deleteOnExitList_().push_back(path_);
    }

    jxx::Ptr<jxx::JxxArray<jxx::Ptr<jxx::lang::String>, 1U>> File::list() const
    {
        if (!isDirectory())
            return nullptr;

        std::vector<jxx::Ptr<jxx::lang::String>> names;
        std::error_code ec;

        for (const auto& entry : fs::directory_iterator(path_, ec))
        {
            if (ec)
                break;
            names.push_back(toStringPtr_(entry.path().filename().generic_string()));
        }

        auto out = std::make_shared<jxx::JxxArray<jxx::Ptr<jxx::lang::String>, 1U>>(
            static_cast<jxx::lang::jint>(names.size()));

        for (std::size_t i = 0; i < names.size(); ++i)
            (*out)(static_cast<jxx::lang::jint>(i)) = names[i];

        return out;
    }

    jxx::Ptr<jxx::JxxArray<jxx::Ptr<File>, 1U>> File::listFiles() const
    {
        if (!isDirectory())
            return nullptr;

        std::vector<jxx::Ptr<File>> files;
        std::error_code ec;

        for (const auto& entry : fs::directory_iterator(path_, ec))
        {
            if (ec)
                break;
            files.push_back(std::make_shared<File>(toStringPtr_(entry.path().generic_string())));
        }

        auto out = std::make_shared<jxx::JxxArray<jxx::Ptr<File>, 1U>>(
            static_cast<jxx::lang::jint>(files.size()));

        for (std::size_t i = 0; i < files.size(); ++i)
            (*out)(static_cast<jxx::lang::jint>(i)) = files[i];

        return out;
    }

    jxx::lang::jbool File::mkdir()
    {
        std::error_code ec;
        return fs::create_directory(path_, ec) && !ec;
    }

    jxx::lang::jbool File::mkdirs()
    {
        std::error_code ec;
        return fs::create_directories(path_, ec) && !ec;
    }

    jxx::lang::jbool File::renameTo(jxx::Ptr<File> dest)
    {
        if (!dest)
            throwIAE_("null destination");

        std::error_code ec;
        fs::rename(path_, dest->path_, ec);
        return !ec;
    }

    jxx::lang::jbool File::setLastModified(jxx::lang::jlong time)
    {
        std::error_code ec;
        const auto tp = std::chrono::system_clock::time_point(std::chrono::milliseconds(time));
        const auto ftp = fs::file_time_type::clock::now() + (tp - std::chrono::system_clock::now());
        fs::last_write_time(path_, ftp, ec);
        return !ec;
    }

    jxx::lang::jbool File::setReadOnly()
    {
    #if defined(_WIN32)
        return setWritable(false);
    #else
        std::error_code ec;
        auto perms = fs::status(path_, ec).permissions();
        if (ec)
            return false;

        perms &= ~fs::perms::owner_write;
        perms &= ~fs::perms::group_write;
        perms &= ~fs::perms::others_write;
        fs::permissions(path_, perms, ec);
        return !ec;
    #endif
    }

    jxx::lang::jbool File::setWritable(jxx::lang::jbool writable)
    {
        std::error_code ec;
        auto perms = fs::status(path_, ec).permissions();
        if (ec)
            return false;

        if (writable)
            perms |= fs::perms::owner_write;
        else
            perms &= ~fs::perms::owner_write;

        fs::permissions(path_, perms, ec);
        return !ec;
    }

    jxx::lang::jbool File::setReadable(jxx::lang::jbool readable)
    {
        std::error_code ec;
        auto perms = fs::status(path_, ec).permissions();
        if (ec)
            return false;

        if (readable)
            perms |= fs::perms::owner_read;
        else
            perms &= ~fs::perms::owner_read;

        fs::permissions(path_, perms, ec);
        return !ec;
    }

    jxx::lang::jbool File::setExecutable(jxx::lang::jbool executable)
    {
        std::error_code ec;
        auto perms = fs::status(path_, ec).permissions();
        if (ec)
            return false;

        if (executable)
            perms |= fs::perms::owner_exec;
        else
            perms &= ~fs::perms::owner_exec;

        fs::permissions(path_, perms, ec);
        return !ec;
    }

    jxx::lang::jint File::compareTo(jxx::Ptr<File> pathname) const
    {
        if (!pathname)
            return 1;

        if (path_ < pathname->path_)
            return -1;
        if (path_ > pathname->path_)
            return 1;
        return 0;
    }

    jxx::lang::jbool File::equals(jxx::Ptr<jxx::lang::Object> other) const
    {
        auto f = std::dynamic_pointer_cast<File>(other);
        return f && f->path_ == path_;
    }

    jxx::lang::jint File::hashCode() const
    {
        return static_cast<jxx::lang::jint>(std::hash<std::string>{}(path_));
    }

    jxx::Ptr<jxx::lang::String> File::toString() const
    {
        return toStringPtr_(path_);
    }

    jxx::Ptr<File> File::createTempFile(jxx::Ptr<jxx::lang::String> prefix,
                                        jxx::Ptr<jxx::lang::String> suffix)
    {
        return createTempFile(prefix, suffix, nullptr);
    }

    jxx::Ptr<File> File::createTempFile(jxx::Ptr<jxx::lang::String> prefix,
                                        jxx::Ptr<jxx::lang::String> suffix,
                                        jxx::Ptr<File> directory)
    {
        const std::string pfx = toUtf8_(prefix);
        const std::string sfx = suffix ? suffix->utf8() : std::string(".tmp");

        if (pfx.size() < 3)
            throwIAE_("prefix too short");

        const fs::path dir = directory ? fs::path(directory->path_) : fs::temp_directory_path();

        for (int i = 0; i < 1000; ++i)
        {
            const auto candidate = dir / fs::path(pfx + std::to_string(std::rand()) + sfx);
            std::ofstream out(candidate.string(), std::ios::binary);
            if (out.good())
                return std::make_shared<File>(toStringPtr_(candidate.generic_string()));
        }

        throw std::runtime_error("failed to create temp file");
    }

    jxx::Ptr<jxx::JxxArray<jxx::Ptr<File>, 1U>> File::listRoots()
    {
    #if defined(_WIN32)
        auto out = std::make_shared<jxx::JxxArray<jxx::Ptr<File>, 1U>>(0);
        return out;
    #else
        auto out = std::make_shared<jxx::JxxArray<jxx::Ptr<File>, 1U>>(1);
        (*out)[0] = std::make_shared<File>(std::make_shared<jxx::lang::String>("/"));
        return out;
    #endif
    }

    void File::writeObject(jxx::Ptr<jxx::io::ObjectOutputStream> out) {
        // Default serialization: write the path string
		///out->writeUTF(toUtf8_(getPath()));
    }
    void File::readObject(jxx::Ptr<jxx::io::ObjectInputStream> in) {
        // Default deserialization: read the path string
        //auto path = in->readUTF();
        /*if (path) {
            path_ = path->utf8();
        } else {
            throw std::runtime_error("Failed to read File path during deserialization");
		}*/
    }

    void File::readObjectNoData() {
    }
}
