#pragma once

#if defined(_WIN32)
typedef struct _iobuf FILE;
#else
typedef struct _IO_FILE FILE;
#endif

#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::io
{
    class FileInputStream;
    class FileOutputStream;

    class FileDescriptor final
        : public ::jxx::lang::ClassBase<
              FileDescriptor,
              ::jxx::lang::Object>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<
            FileDescriptor,
            JxxSuper>;

        static ::jxx::Ptr<FileDescriptor> in;
        static ::jxx::Ptr<FileDescriptor> out;
        static ::jxx::Ptr<FileDescriptor> err;

        FileDescriptor();
        explicit FileDescriptor(
            FILE* handle,
            ::jxx::lang::jbool owned = false);
        ~FileDescriptor() override;

        ::jxx::lang::jbool valid() const;
        void sync();
        FILE* nativeHandle() const;

    private:
        friend class FileInputStream;
        friend class FileOutputStream;

        void invalidate(FILE* expectedHandle) noexcept;
        ::jxx::lang::jbool closeConnection(FILE* expectedHandle) noexcept;

        FILE* handle_ = nullptr;
        ::jxx::lang::jbool owned_ = false;
    };
} // namespace jxx::io
