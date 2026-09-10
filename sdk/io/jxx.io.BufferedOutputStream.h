#pragma once

#include "io/jxx.io.FilterOutputStream.h"
#include "lang/jxx.lang.buildin_array.h"
#include "lang/jxx_types.h"

namespace jxx::io {

class BufferedOutputStream
    : public ::jxx::lang::ClassBase<
          BufferedOutputStream,
          FilterOutputStream> {
public:
    using JxxSuper = FilterOutputStream;

    using Super =
        ::jxx::lang::ClassBase<
            BufferedOutputStream,
            JxxSuper>;

    explicit BufferedOutputStream(
        const ::jxx::Ptr<OutputStream>& output);

    BufferedOutputStream(
        const ::jxx::Ptr<OutputStream>& output,
        ::jxx::lang::jint size);

    ~BufferedOutputStream() override;

    void write(
        ::jxx::lang::jint value) override;

    void write(
        const ::jxx::lang::ByteArray& buffer,
        ::jxx::lang::jint offset,
        ::jxx::lang::jint length) override;

    void flush() override;
    void close() override;

private:
    static ::jxx::lang::jint validateBufferSize_(
        ::jxx::lang::jint size);

    void ensureOpen_() const;
    void flushBuffer_();

    ::jxx::lang::ByteArray buffer_;
    ::jxx::lang::jint count_ = 0;
    ::jxx::lang::jbool closed_ = false;
};

} // namespace jxx::io
