#include "io/jxx.io.FileDescriptor.h"

#include <stdexcept>
#include <string>

#if !defined(_WIN32)
    #include <unistd.h>
#endif

#include "jxx.lang.String.h"

namespace jxx::io
{
    jxx::Ptr<FileDescriptor> FileDescriptor::in  = std::make_shared<FileDescriptor>(0);
    jxx::Ptr<FileDescriptor> FileDescriptor::out = std::make_shared<FileDescriptor>(1);
    jxx::Ptr<FileDescriptor> FileDescriptor::err = std::make_shared<FileDescriptor>(2);

    FileDescriptor::FileDescriptor() = default;

    FileDescriptor::FileDescriptor(jxx::lang::jint fd)
        : fd_(fd)
    {
    }

    jxx::lang::jbool FileDescriptor::valid() const
    {
        return fd_ >= 0;
    }

    void FileDescriptor::sync()
    {
        if (fd_ < 0)
            throw std::runtime_error("invalid file descriptor");

    #if !defined(_WIN32)
        if (::fsync(fd_) != 0)
            throw std::runtime_error("fsync failed");
    #else
        // Windows implementation can be added later if needed.
    #endif
    }

    jxx::Ptr<jxx::lang::String> FileDescriptor::toString() const
    {
        return std::make_shared<jxx::lang::String>(
            std::string("FileDescriptor[fd=") + std::to_string(fd_) + "]");
    }

    jxx::lang::jbool FileDescriptor::equals(jxx::Ptr<jxx::lang::Object> other) const
    {
        auto fd = std::dynamic_pointer_cast<FileDescriptor>(other);
        return fd && fd->fd_ == fd_;
    }

    jxx::lang::jint FileDescriptor::hashCode() const
    {
        return fd_;
    }
}
