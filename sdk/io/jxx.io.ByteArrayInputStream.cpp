#include "jxx.io.ByteArrayInputStream.h"

namespace jxx::io {

    ByteArrayInputStream::ByteArrayInputStream(jxx::Ptr<ByteArray> buf)
        : ByteArrayInputStream(buf, 0, buf ? (jxx::lang::jint)buf->length : 0) {}

    ByteArrayInputStream::ByteArrayInputStream(jxx::Ptr<ByteArray> buf, jxx::lang::jint offset, jxx::lang::jint length)
        : buf_(std::move(buf)) {
        if (!buf_) throw jxx::lang::NullPointerException(JXX_NEW<jxx::lang::String>("buf"));
        pos_ = offset;
        mark_ = offset;
        end_ = offset + length;
    }

    jxx::lang::jint ByteArrayInputStream::read() {
        if (pos_ >= end_) return -1;
        return ((jxx::lang::jint)(*buf_)[pos_++]) & 0xFF;
    }

    jxx::lang::jint ByteArrayInputStream::read(jxx::Ptr<ByteArray> b, jxx::lang::jint off, jxx::lang::jint len) {
        if (pos_ >= end_) return -1;
        jxx::lang::jint n = std::min(len, end_ - pos_);
        for (jxx::lang::jint i = 0; i < n; ++i) {
            (*b)[off + i] = (*buf_)[pos_ + i];
        }
        pos_ += n;
        return n;
    }

    jxx::lang::jlong ByteArrayInputStream::skip(jxx::lang::jlong n) {
        jxx::lang::jint k = std::min((jxx::lang::jint)n, end_ - pos_);
        pos_ += k;
        return k;
    }

    jxx::lang::jint ByteArrayInputStream::available() {
        return end_ - pos_;
    }

    void ByteArrayInputStream::mark(jxx::lang::jint) {
        mark_ = pos_;
    }

    void ByteArrayInputStream::reset() {
        pos_ = mark_;
    }

    jxx::lang::jbool ByteArrayInputStream::markSupported() {
        return true;
    }

} // namespace jxx::io