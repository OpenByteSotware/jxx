#pragma once

#include <cstdio>

#include "lang/jxx_types.h"
#include "jxx.io.OutputStream.h"

namespace jxx::io
{
    class File;
    class FileDescriptor;

    class FileOutputStream final : public OutputStream
    {
    public:
        explicit FileOutputStream(const jxx::Ptr<jxx::lang::String> name);
        FileOutputStream(const jxx::Ptr<jxx::lang::String> name, jxx::lang::jbool append);
        explicit FileOutputStream(const jxx::Ptr<File> file);
        FileOutputStream(const jxx::Ptr<File> file, jxx::lang::jbool append);
        explicit FileOutputStream(const jxx::Ptr<FileDescriptor> fdObj);

        ~FileOutputStream() override;

    public:
        void write(jxx::lang::jint b) override;
        void write(const jxx::lang::ByteArray b,
                   jxx::lang::jint off,
                   jxx::lang::jint len) override;
        void flush() override;
        void close() override;

        jxx::Ptr<FileDescriptor> getFD() const;

    private:
        void openPath_(const std::string& path, bool append);

    private:
        std::FILE* file_ = nullptr;
        jxx::Ptr<FileDescriptor> fd_;
    };
}
