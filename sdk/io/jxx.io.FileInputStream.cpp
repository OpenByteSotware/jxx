#include "io/jxx.io.FileInputStream.h"

#include <stdexcept>
#include <string>
#include <vector>

#if !defined(_WIN32)
    #include <unistd.h>
#endif

#include "lang/jxx.lang.String.h"
#include "io/jxx.io.File.h"
#include "io/jxx.io.FileDescriptor.h"

namespace
{
    [[noreturn]] void throwIOE_(const char* msg)
    {
        throw std::runtime_error(msg);
    }
}

namespace jxx::io
{
    FileInputStream::FileInputStream(jxx::Ptr<jxx::lang::String> name)
    {
        if (!name)
            throwIOE_("null file name");

        openPath_(name->utf8());
    }

    FileInputStream::FileInputStream(jxx::Ptr<File> file)
    {
        if (!file)
            throwIOE_("null file");

        openPath_(file->getPath()->utf8());
    }

    FileInputStream::FileInputStream(jxx::Ptr<FileDescriptor> fdObj)
        : fd_(std::move(fdObj))
    {
        throwIOE_("FileInputStream(FileDescriptor) needs native descriptor integration");
    }

    FileInputStream::~FileInputStream()
    {
        close();
    }

    void FileInputStream::openPath_(const std::string& path)
    {
        file_ = std::fopen(path.c_str(), "rb");
        if (!file_)
            throwIOE_("failed to open file");

    #if !defined(_WIN32)
        fd_ = jxx::NEW<FileDescriptor>();
        fd_->setNativeFd_(::fileno(file_));
    #endif
    }

    jxx::lang::jint FileInputStream::read()
    {
        if (!file_)
            return -1;

        const int c = std::fgetc(file_);
        return (c == EOF) ? -1 : c;
    }

    jxx::lang::jint FileInputStream::read(const jxx::lang::ByteArray b,
                                          jxx::lang::jint off,
                                          jxx::lang::jint len)
    {
        if (!file_)
            return -1;
        if (!b)
            throwIOE_("null byte array");
        if (off < 0 || len < 0 || off > b->length - len)
            throwIOE_("index out of bounds");
        if (len == 0)
            return 0;

        std::vector<unsigned char> tmp(static_cast<std::size_t>(len));
        const std::size_t n = std::fread(tmp.data(), 1, tmp.size(), file_);
        if (n == 0)
            return -1;

        for (std::size_t i = 0; i < n; ++i)
            (*b)[off + static_cast<jxx::lang::jint>(i)] = static_cast<jxx::lang::jbyte>(tmp[i]);

        return static_cast<jxx::lang::jint>(n);
    }

    jxx::lang::jlong FileInputStream::skip(jxx::lang::jlong n)
    {
        if (!file_ || n <= 0)
            return 0;

        const auto cur = std::ftell(file_);
        std::fseek(file_, static_cast<long>(n), SEEK_CUR);
        const auto now = std::ftell(file_);
        return static_cast<jxx::lang::jlong>(now - cur);
    }

    jxx::lang::jint FileInputStream::available()
    {
        return 0;
    }

    void FileInputStream::close()
    {
        if (file_)
        {
            std::fclose(file_);
            file_ = nullptr;
        }
    }

    jxx::Ptr<FileDescriptor> FileInputStream::getFD() const
    {
        return fd_;
    }
}
