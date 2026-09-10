#pragma once

#include "io/jxx.io.Writer.h"
#include "lang/jxx.lang.buildin_array.h"
#include "lang/jxx_types.h"

namespace jxx::lang {
class String;
}

namespace jxx::io {

class BufferedWriter
    : public ::jxx::lang::ClassBase<
          BufferedWriter,
          Writer> {
public:
    using JxxSuper = Writer;

    using Super =
        ::jxx::lang::ClassBase<
            BufferedWriter,
            JxxSuper>;

    using JxxSuper::write;

    explicit BufferedWriter(
        const ::jxx::Ptr<Writer>& output);

    BufferedWriter(
        const ::jxx::Ptr<Writer>& output,
        ::jxx::lang::jint size);

    ~BufferedWriter() override;

    void write(
        ::jxx::lang::jint value) override;

    void write(
        const ::jxx::lang::CharArray& buffer,
        ::jxx::lang::jint offset,
        ::jxx::lang::jint length) override;

    void write(
        const ::jxx::Ptr<::jxx::lang::String>& value,
        ::jxx::lang::jint offset,
        ::jxx::lang::jint length) override;

    void newLine();
    void flush() override;
    void close() override;

private:
    static ::jxx::lang::jint validateBufferSize_(
        ::jxx::lang::jint size);

    void ensureOpen_() const;
    void flushBuffer_();

    ::jxx::Ptr<Writer> output_;
    ::jxx::lang::CharArray buffer_;
    ::jxx::lang::jint count_ = 0;
    ::jxx::lang::jbool closed_ = false;
};

} // namespace jxx::io
