#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.StringBuilder.h"
#include "jxx.io.IOHelper.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "jxx.io.BufferedReader.h"

namespace jxx::io {

BufferedReader::BufferedReader(jxx::Ptr<Reader> in, jxx::lang::jint size)
    : in_(std::move(in)) {
    if (!in_) throw jxx::lang::NullPointerException(jxx::NEW<jxx::lang::String>("in"));
    if (size <= 0) throw jxx::lang::IllegalArgumentException(jxx::NEW<jxx::lang::String>("size"));
    buf_ = jxx::NEW<jxx::lang::CharArrayType>((std::uint32_t)size);
}

void BufferedReader::fill_() {
    count_ = in_->read(buf_, 0, (jxx::lang::jint)buf_->length);
    pos_ = 0;
}

jxx::lang::jint BufferedReader::read() {
    if (pos_ >= count_) {
        fill_();
        if (count_ <= 0) return -1;
    }
    return (jxx::lang::jint)(*buf_)[pos_++];
}

jxx::lang::jint BufferedReader::read(const jxx::lang::CharArray cbuf, jxx::lang::jint off, jxx::lang::jint len) {
    IOHelper::checkBounds_(cbuf, off, len);
    if (len == 0) return 0;

    jxx::lang::jint written = 0;
    while (written < len) {
        jxx::lang::jint c = read();
        if (c < 0) return (written == 0) ? -1 : written;
        (*cbuf)[off + written] = (jxx::lang::jchar)c;
        ++written;
    }
    return written;
}

jxx::Ptr<jxx::lang::String> BufferedReader::readLine() {
    auto sb = jxx::NEW<jxx::lang::StringBuilder>();

    jxx::lang::jint c = read();
    if (c < 0) return nullptr;

    while (c >= 0) {
        if (c == (jxx::lang::jchar)'\r') break;
        if (c == (jxx::lang::jchar)'\n') {
            jxx::lang::jint n = read();
            if (n != (jxx::lang::jchar)'\n' && n >= 0) {
                if (pos_ > 0) --pos_;
            }
            break;
        }
        sb->append((jxx::lang::jchar)c);
        c = read();
    }
    return sb->toString();
}

jxx::lang::jbool BufferedReader::ready() {
    return (pos_ < count_) || in_->ready();
}

void BufferedReader::close() {
    if (in_) in_->close();
    in_ = nullptr;
    buf_ = nullptr;
    pos_ = count_ = 0;
}

} // namespace jxx::io
