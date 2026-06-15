#include "io/jxx.io.FileOutputStream.h"

#include <stdexcept>
#include <string>
#include <vector>

#if !defined(_WIN32)
    #include <unistd.h>
#endif

#include "jxx.lang.String.h"
#include "io/jxx.io.File.h"
#include "io/jxx.io.FileDescriptor.h"

namespace
{
    [[noreturn]] void throwIOE_(const char* msg)
    {
        throw std::runtime_error(msg);
    }

    [[noreturn]] void throwIAE_(const char* msg)
    {
        throw std::invalid_argument(msg);
    }
}

namespace jxx::io
{
    FileOutputStream::FileOutputStream(jxx::Ptr<jxx::lang::String> name)
    {
        if (!name)
            throwIOE_("null file name");
        openPath_(name->utf8(), false);
    }

    FileOutputStream::FileOutputStream(jxx::Ptr<jxx::lang::String> name, jxx::lang::jbool append)
    {
        if (!name)
            throwIOE_("null file name");
        openPath_(name->utf8(), append);
    }

    FileOutputStream::FileOutputStream(jxx::Ptr<File> file)
    {
        if (!file)
            throwIOE_("null file");
        openPath_(file->getPath()->utf8(), false);
    }

    FileOutputStream::FileOutputStream(jxx::Ptr<File> file, jxx::lang::jbool append)
    {
        if (!file)
            throwIOE_("null file");
        openPath_(file->getPath()->utf8(), append);
    }

    FileOutputStream::FileOutputStream(jxx::Ptr<FileDescriptor> fdObj)
        : fd_(std::move(fdObj))
    {
        throwIOE_("FileOutputStream(FileDescriptor) needs native descriptor integration");
    }

    FileOutputStream::~FileOutputStream()
    {
        close();
    }

    void FileOutputStream::openPath_(const std::string& path, bool append)
    {
        file_ = std::fopen(path.c_str(), append ? "ab" : "wb");
        if (!file_)
            throwIOE_("failed to open file");

    #if !defined(_WIN32)
        fd_ = std::make_shared<FileDescriptor>();
        fd_->setNativeFd_(::fileno(file_));
    #endif
    }

    void FileOutputStream::write(jxx::lang::jint b)
    {
        if (!file_)
            throwIOE_("stream closed");

        if (std::fputc(static_cast<unsigned char>(b), file_) == EOF)
            throwIOE_("write failed");
    }

    void FileOutputStream::write(const jxx::lang::ByteArray b,
                                 jxx::lang::jint off,
                                 jxx::lang::jint len)
    {
        if (!file_)
            throwIOE_("stream closed");
        if (!b)
            throwIAE_("null byte array");
        if (off < 0 || len < 0 || off > b->length() - len)
            throwIAE_("index out of bounds");

        std::vector<unsigned char> tmp(static_cast<std::size_t>(len));
        for (jxx::lang::jint i = 0; i < len; ++i)
            tmp[static_cast<std::size_t>(i)] = static_cast<unsigned char>((*b)[off + i]);

        const std::size_t n = std::fwrite(tmp.data(), 1, tmp.size(), file_);
        if (n != tmp.size())
            throwIOE_("write failed");
    }

    void FileOutputStream::flush()
    {
        if (file_)
            std::fflush(file_);
    }

    void FileOutputStream::close()
    {
        if (file_)
        {
            std::fflush(file_);
            std::fclose(file_);
            file_ = nullptr;
        }
    }

    jxx::Ptr<FileDescriptor> FileOutputStream::getFD() const
    {
        return fd_;
    }
}
