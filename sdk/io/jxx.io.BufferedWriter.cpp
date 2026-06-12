#include "jxx.io.BufferedWriter.h"

namespace jxx::io {

static constexpr jxx::lang::jint DEFAULT_BUFSZ = 8192;

BufferedWriter::BufferedWriter(jxx::Ptr<Writer> out)
    : BufferedWriter(std::move(out), DEFAULT_BUFSZ) {}

BufferedWriter::BufferedWriter(jxx::Ptr<Writer> out, jxx::lang::jint sz)
    : out_(std::move(out)) {
    if (!out_) throw jxx::lang::NullPointerException(jxx::NEW<jxx::lang::String>("out"));
    if (sz <= 0) throw jxx::lang::IllegalArgumentException(jxx::NEW<jxx::lang::String>("sz"));
    buf_ = jxx::NEW<jxx::lang::CharArrayType>((std::uint32_t)sz);
}

void BufferedWriter::flushBuffer_() {
    if (count_ > 0) {
        out_->write(buf_, 0, count_);
        count_ = 0;
    }
}

void BufferedWriter::write(jxx::lang::jint c) {
    if (count_ >= (jxx::lang::jint)buf_->length) flushBuffer_();
    (*buf_)[count_++] = (jxx::lang::jchar)(c & 0xFFFF);
}

void BufferedWriter::write(const jxx::lang::CharArray cbuf, jxx::lang::jint off, jxx::lang::jint len) {
    Writer::checkBounds_(cbuf, off, len);

    if (len >= (jxx::lang::jint)buf_->length) {
        flushBuffer_();
        out_->write(cbuf, off, len);
        return;
    }

    if (len > (jxx::lang::jint)buf_->length - count_) flushBuffer_();
    for (jxx::lang::jint i = 0; i < len; ++i) (*buf_)[count_ + i] = (*cbuf)[off + i];
    count_ += len;
}

void BufferedWriter::write(jxx::Ptr<jxx::lang::String> str, jxx::lang::jint off, jxx::lang::jint len) {
    Writer::checkStringBounds_(str, off, len);
    const auto& u16 = str->utf16();
    // write via temp buffer
    auto tmp = jxx::NEW<jxx::lang::CharArrayType>((std::uint32_t)len);
    for (jxx::lang::jint i = 0; i < len; ++i) (*tmp)[i] = (jxx::lang::jchar)u16[(std::size_t)(off + i)];
    write(tmp, 0, len);
}

void BufferedWriter::newLine() {
    // Java uses line.separator; we use \n
    write((jxx::lang::jint)'\n');
}

void BufferedWriter::flush() {
    flushBuffer_();
    out_->flush();
}

void BufferedWriter::close() {
    try { flush(); } catch (...) {}
    out_->close();
}

} // namespace jxx::io
