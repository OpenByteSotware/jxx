#include <cstdio>

#include "io/jxx.io.SyncFailedException.h"
#include "io/jxx.io.FileDescriptor.h"

namespace jxx::io
{
    ::jxx::Ptr<FileDescriptor> FileDescriptor::in =
        ::jxx::NEW<FileDescriptor>(stdin, false);
    ::jxx::Ptr<FileDescriptor> FileDescriptor::out =
        ::jxx::NEW<FileDescriptor>(stdout, false);
    ::jxx::Ptr<FileDescriptor> FileDescriptor::err =
        ::jxx::NEW<FileDescriptor>(stderr, false);

    FileDescriptor::FileDescriptor() = default;

    FileDescriptor::FileDescriptor(
        FILE* handle,
        ::jxx::lang::jbool owned)
        : handle_(handle)
        , owned_(owned)
    {
    }

    FileDescriptor::~FileDescriptor()
    {
        try {
            synchronized([&] {
                if (owned_ && handle_) {
                    std::fclose(handle_);
                }
                handle_ = nullptr;
                owned_ = false;
            });
        } catch (...) {
        }
    }

    ::jxx::lang::jbool FileDescriptor::valid() const
    {
        return synchronized([&]() -> ::jxx::lang::jbool {
            return handle_ != nullptr;
        });
    }

    void FileDescriptor::sync()
    {
        synchronized([&] {
            if (!handle_ || std::fflush(handle_) != 0) {
                throw SyncFailedException();
            }
        });
    }

    FILE* FileDescriptor::nativeHandle() const
    {
        return synchronized([&]() -> FILE* {
            return handle_;
        });
    }

    void FileDescriptor::invalidate(FILE* expectedHandle) noexcept
    {
        try {
            synchronized([&] {
                if (handle_ == expectedHandle) {
                    handle_ = nullptr;
                    owned_ = false;
                }
            });
        } catch (...) {
        }
    }
} // namespace jxx::io
