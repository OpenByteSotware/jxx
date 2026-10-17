#pragma once
#include "ext/imageio/stream/jxx.ext.imageio.stream.ImageInputStream.h"
#include "io/jxx.io.DataOutput.h"
#include "io/jxx.io.Flushable.h"
namespace jxx::ext::imageio::stream {
class ImageOutputStream : public ::jxx::lang::InterfaceBase<
    ImageOutputStream, ImageInputStream, ::jxx::io::DataOutput,
    ::jxx::io::Flushable> {
public:
    ~ImageOutputStream() override = default;
    virtual void writeBit(::jxx::lang::jint bit) = 0;
    virtual void writeBits(::jxx::lang::jlong bits,
                           ::jxx::lang::jint count) = 0;
};
} // namespace jxx::ext::imageio::stream
