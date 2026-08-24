#pragma once

#include <cstdio>

#include "lang/jxx_types.h"
#include "jxx.io.InputStream.h"

namespace jxx::io
{
    class File;
    class FileDescriptor;

    class FileInputStream final : public InputStream
    {
    public:
        explicit FileInputStream(const jxx::Ptr<jxx::lang::String> name);
        explicit FileInputStream(const jxx::Ptr<File> file);
        explicit FileInputStream(const jxx::Ptr<FileDescriptor> fdObj);

        ~FileInputStream() override;

    public:
        jxx::lang::jint read() override;
        jxx::lang::jint read(const jxx::lang::ByteArray b,
                             jxx::lang::jint off,
                             jxx::lang::jint len) override;
        jxx::lang::jlong skip(jxx::lang::jlong n) override;
        jxx::lang::jint available() override;
        void close() override;

        jxx::Ptr<FileDescriptor> getFD() const;

    private:
        void openPath_(const std::string& path);

    private:
        std::FILE* file_ = nullptr;
        jxx::Ptr<FileDescriptor> fd_;
    };
}
