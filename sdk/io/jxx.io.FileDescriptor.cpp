#include <cstdio>
#if defined(_WIN32)
#include <io.h>
#else
#include <unistd.h>
#endif

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
#if defined(_WIN32)
            const auto descriptor = ::_fileno(handle_);
            if (descriptor < 0 || ::_commit(descriptor) != 0) {
                throw SyncFailedException();
            }
#else
            const auto descriptor = ::fileno(handle_);
            if (descriptor < 0 || ::fsync(descriptor) != 0) {
                throw SyncFailedException();
            }
#endif
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
