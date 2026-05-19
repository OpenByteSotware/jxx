#include "jxx.io.BufferedOutputStream.h"

namespace jxx::io {

static constexpr jxx::lang::jint DEFAULT_BUF_SIZE = 8192;

BufferedOutputStream::BufferedOutputStream(jxx::Ptr<OutputStream> out)
    : BufferedOutputStream(std::move(out), DEFAULT_BUF_SIZE) {}

BufferedOutputStream::BufferedOutputStream(jxx::Ptr<OutputStream> out, jxx::lang::jint size)
    : FilterOutputStream(std::move(out)) {
    if (size <= 0) throw jxx::lang::IllegalArgumentException(JXX_NEW<jxx::lang::String>("size <= 0"));
    buf_ = JXX_NEW<ByteArray>((std::uint32_t)size);
}

void BufferedOutputStream::flushBuffer_() {
    if (count_ > 0) {
        out_->write(buf_, 0, count_);
        count_ = 0;
    }
}

void BufferedOutputStream::write(jxx::lang::jint b) {
    if (count_ >= (jxx::lang::jint)buf_->length) flushBuffer_();
    (*buf_)[count_++] = (jxx::lang::jbyte)(b & 0xFF);
}

void BufferedOutputStream::write(jxx::Ptr<ByteArray> b, jxx::lang::jint off, jxx::lang::jint len) {
    OutputStream::checkBounds_(b, off, len);

    if (len >= (jxx::lang::jint)buf_->length) {
        flushBuffer_();
        out_->write(b, off, len);
        return;
    }

    if (len > (jxx::lang::jint)buf_->length - count_) flushBuffer_();

    for (jxx::lang::jint i = 0; i < len; ++i) {
        (*buf_)[count_ + i] = (*b)[off + i];
    }
    count_ += len;
}

void BufferedOutputStream::flush() {
    flushBuffer_();
    out_->flush();
}

void BufferedOutputStream::close() {
    try { flush(); } catch (...) {}
    out_->close();
}

} // namespace jxx::io
