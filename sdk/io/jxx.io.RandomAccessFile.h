#pragma once

#include <cstdio>
#include <mutex>

#include "io/jxx.io.Closeable.h"
#include "io/jxx.io.DataInput.h"
#include "io/jxx.io.DataOutput.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.buildin_array.h"
#include "lang/jxx_types.h"

namespace jxx::lang { class String; }
namespace jxx::nio::channels { class FileChannel; }

namespace jxx::io {

class File;
class FileDescriptor;

class RandomAccessFile final
    : public ::jxx::lang::ClassBase<
          RandomAccessFile,
          ::jxx::lang::Object,
          DataOutput,
          DataInput,
          Closeable> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<
        RandomAccessFile,
        JxxSuper,
        DataOutput,
        DataInput,
        Closeable>;

    RandomAccessFile(
        const ::jxx::Ptr<::jxx::lang::String>& name,
        const ::jxx::Ptr<::jxx::lang::String>& mode);
    RandomAccessFile(
        const ::jxx::Ptr<File>& file,
        const ::jxx::Ptr<::jxx::lang::String>& mode);
    ~RandomAccessFile() override;

    ::jxx::Ptr<FileDescriptor> getFD();
    ::jxx::Ptr<::jxx::nio::channels::FileChannel> getChannel();

    ::jxx::lang::jint read();
    ::jxx::lang::jint read(const ::jxx::lang::ByteArray& buffer);
    ::jxx::lang::jint read(
        const ::jxx::lang::ByteArray& buffer,
        ::jxx::lang::jint offset,
        ::jxx::lang::jint length);

    void readFully(const ::jxx::lang::ByteArray& buffer) override;
    void readFully(
        const ::jxx::lang::ByteArray& buffer,
        ::jxx::lang::jint offset,
        ::jxx::lang::jint length) override;
    ::jxx::lang::jint skipBytes(::jxx::lang::jint count) override;
    ::jxx::lang::jbool readBoolean() override;
    ::jxx::lang::jbyte readByte() override;
    ::jxx::lang::jint readUnsignedByte() override;
    ::jxx::lang::jshort readShort() override;
    ::jxx::lang::jint readUnsignedShort() override;
    ::jxx::lang::jchar readChar() override;
    ::jxx::lang::jint readInt() override;
    ::jxx::lang::jlong readLong() override;
    ::jxx::lang::jfloat readFloat() override;
    ::jxx::lang::jdouble readDouble() override;
    ::jxx::Ptr<::jxx::lang::String> readLine() override;
    ::jxx::Ptr<::jxx::lang::String> readUTF() override;

    void write(::jxx::lang::jint value) override;
    void write(const ::jxx::lang::ByteArray& buffer) override;
    void write(
        const ::jxx::lang::ByteArray& buffer,
        ::jxx::lang::jint offset,
        ::jxx::lang::jint length) override;
    void writeBoolean(::jxx::lang::jbool value) override;
    void writeByte(::jxx::lang::jint value) override;
    void writeShort(::jxx::lang::jint value) override;
    void writeChar(::jxx::lang::jint value) override;
    void writeInt(::jxx::lang::jint value) override;
    void writeLong(::jxx::lang::jlong value) override;
    void writeFloat(::jxx::lang::jfloat value) override;
    void writeDouble(::jxx::lang::jdouble value) override;
    void writeBytes(const ::jxx::Ptr<::jxx::lang::String>& value) override;
    void writeChars(const ::jxx::Ptr<::jxx::lang::String>& value) override;
    void writeUTF(const ::jxx::Ptr<::jxx::lang::String>& value) override;

    ::jxx::lang::jlong getFilePointer();
    void seek(::jxx::lang::jlong position);
    ::jxx::lang::jlong length();
    void setLength(::jxx::lang::jlong newLength);
    void close() override;

private:
    void open_(
        const ::jxx::Ptr<File>& file,
        const ::jxx::Ptr<::jxx::lang::String>& mode);
    void ensureOpen_() const;
    void ensureWritable_() const;
    void validateRange_(
        const ::jxx::lang::ByteArray& buffer,
        ::jxx::lang::jint offset,
        ::jxx::lang::jint length) const;
    ::jxx::lang::jint readRequiredByte_();
    void syncIfRequired_();

    mutable std::recursive_mutex mutex_;
    ::jxx::Ptr<File> file_;
    ::jxx::Ptr<FileDescriptor> descriptor_;
    ::jxx::Ptr<::jxx::nio::channels::FileChannel> channel_;
    FILE* handle_ = nullptr;
    ::jxx::lang::jbool writable_ = false;
    ::jxx::lang::jbool syncData_ = false;
    ::jxx::lang::jbool syncMetadata_ = false;
};

} // namespace jxx::io
