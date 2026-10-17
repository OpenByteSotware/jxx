#pragma once
#include <vector>
#include "ext/imageio/stream/jxx.ext.imageio.stream.ImageOutputStream.h"
namespace jxx::io { class OutputStream; }
namespace jxx::ext::imageio::stream {
class MemoryCacheImageOutputStream final
    : public ::jxx::lang::ClassBase<MemoryCacheImageOutputStream,
          ::jxx::lang::Object, ImageOutputStream> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<MemoryCacheImageOutputStream,
        JxxSuper, ImageOutputStream>;
    explicit MemoryCacheImageOutputStream(
        const ::jxx::Ptr<::jxx::io::OutputStream>& output);
    ~MemoryCacheImageOutputStream() override;
    ::jxx::lang::jint read() override;
    ::jxx::lang::jint read(const ::jxx::lang::ByteArray& buffer,
        ::jxx::lang::jint offset, ::jxx::lang::jint length) override;
    void readFully(const ::jxx::lang::ByteArray& buffer) override;
    void readFully(const ::jxx::lang::ByteArray& buffer,
        ::jxx::lang::jint offset, ::jxx::lang::jint length) override;
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
    void write(const ::jxx::lang::ByteArray& buffer,
        ::jxx::lang::jint offset, ::jxx::lang::jint length) override;
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
    ::jxx::lang::jlong getStreamPosition() const override;
    ::jxx::lang::jint getBitOffset() const override;
    void setBitOffset(::jxx::lang::jint bitOffset) override;
    ::jxx::lang::jint readBit() override;
    ::jxx::lang::jlong readBits(::jxx::lang::jint count) override;
    void writeBit(::jxx::lang::jint bit) override;
    void writeBits(::jxx::lang::jlong bits, ::jxx::lang::jint count) override;
    void seek(::jxx::lang::jlong position) override;
    void mark() override;
    void reset() override;
    void flushBefore(::jxx::lang::jlong position) override;
    ::jxx::lang::jlong getFlushedPosition() const override;
    ::jxx::lang::jbool isCached() const override;
    ::jxx::lang::jbool isCachedMemory() const override;
    ::jxx::lang::jbool isCachedFile() const override;
    ::jxx::lang::jlong length() const override;
    void flush() override;
    void close() override;
private:
    void ensureOpen_() const;
    void align_();
    std::uint8_t readRaw_();
    void writeRaw_(std::uint8_t value);
    ::jxx::Ptr<::jxx::io::OutputStream> output_;
    std::vector<std::uint8_t> cache_;
    ::jxx::lang::jlong position_{0};
    ::jxx::lang::jlong flushedPosition_{0};
    std::vector<::jxx::lang::jlong> marks_;
    ::jxx::lang::jint bitOffset_{0};
    ::jxx::lang::jbool closed_{false};
};
} // namespace jxx::ext::imageio::stream
