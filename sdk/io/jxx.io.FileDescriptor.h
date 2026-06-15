#pragma once

#include "lang/jxx_types.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::io
{
    class FileInputStream;
    class FileOutputStream;

    /**
     * Java 8 parity: java.io.FileDescriptor
     *
     * Practical POSIX-oriented implementation storing a native integer file
     * descriptor when available.
     */
    class FileDescriptor final : public jxx::lang::Object
    {
    public:
        static jxx::Ptr<FileDescriptor> in;
        static jxx::Ptr<FileDescriptor> out;
        static jxx::Ptr<FileDescriptor> err;

    public:
        FileDescriptor();
        ~FileDescriptor() override = default;

    public:
        jxx::lang::jbool valid() const;
        void sync();

        jxx::Ptr<jxx::lang::String> toString() const override;
        jxx::lang::jbool equals(jxx::Ptr<jxx::lang::Object> other) const override;
        jxx::lang::jint hashCode() const override;

    private:
        explicit FileDescriptor(jxx::lang::jint fd);

        void setNativeFd_(jxx::lang::jint fd) noexcept { fd_ = fd; }
        jxx::lang::jint nativeFd_() const noexcept { return fd_; }

    private:
        jxx::lang::jint fd_ = -1;

    private:
        friend class FileInputStream;
        friend class FileOutputStream;
    };
}
