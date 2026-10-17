#pragma once
#include "io/jxx.io.Closeable.h"
#include "io/jxx.io.DataInput.h"
#include "lang/jxx.lang.ClassInfo.h"
namespace jxx::ext::imageio::stream {
class ImageInputStream : public ::jxx::lang::InterfaceBase<
    ImageInputStream, ::jxx::io::DataInput, ::jxx::io::Closeable> {
public:
    ~ImageInputStream() override = default;
    virtual ::jxx::lang::jint read() = 0;
    virtual ::jxx::lang::jint read(const ::jxx::lang::ByteArray& buffer,
        ::jxx::lang::jint offset, ::jxx::lang::jint length) = 0;
    virtual ::jxx::lang::jlong getStreamPosition() const = 0;
    virtual ::jxx::lang::jint getBitOffset() const = 0;
    virtual void setBitOffset(::jxx::lang::jint bitOffset) = 0;
    virtual ::jxx::lang::jint readBit() = 0;
    virtual ::jxx::lang::jlong readBits(::jxx::lang::jint count) = 0;
    virtual void seek(::jxx::lang::jlong position) = 0;
    virtual void mark() = 0;
    virtual void reset() = 0;
    virtual void flushBefore(::jxx::lang::jlong position) = 0;
    virtual ::jxx::lang::jlong getFlushedPosition() const = 0;
    virtual ::jxx::lang::jbool isCached() const = 0;
    virtual ::jxx::lang::jbool isCachedMemory() const = 0;
    virtual ::jxx::lang::jbool isCachedFile() const = 0;
    virtual ::jxx::lang::jlong length() const = 0;
};
} // namespace jxx::ext::imageio::stream
