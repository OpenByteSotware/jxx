#include "jxx.io.InputStreamReader.h"

namespace jxx::io {

InputStreamReader::InputStreamReader(jxx::Ptr<InputStream> in)
    : InputStreamReader(std::move(in), jxx::lang::Charset::defaultCharset()) {}

InputStreamReader::InputStreamReader(jxx::Ptr<InputStream> in, jxx::Ptr<jxx::lang::Charset> cs)
    : in_(std::move(in)), cs_(std::move(cs)) {
    if (!in_) throw jxx::lang::NullPointerException(jxx::NEW<jxx::lang::String>("in"));
    if (!cs_) throw jxx::lang::NullPointerException(jxx::NEW<jxx::lang::String>("charset"));
}

jbool InputStreamReader::refill_() {
    decoded_.clear();
    dpos_ = 0;

    auto buf = jxx::NEW<ByteArray>(4096);
    jint r = in_->read(buf, 0, (jint)buf->length);
    if (r < 0) return false;

    auto slice = jxx::NEW<ByteArray>((std::uint32_t)r);
    for (jint i = 0; i < r; ++i) (*slice)[i] = (*buf)[i];

    auto s = cs_->decode(slice);
    decoded_ = s ? s->utf16() : std::u16string{};
    return (r > 0);
}

jint InputStreamReader::read() {
    if (dpos_ >= decoded_.size()) {
        if (!refill_()) return -1;
        if (decoded_.empty()) return -1;
    }
    return (jint)decoded_[dpos_++];
}

jint InputStreamReader::read(jxx::Ptr<CharArray> cbuf, jint off, jint len) {
    Reader::checkBounds_(cbuf, off, len);
    if (len == 0) return 0;

    jint written = 0;
    while (written < len) {
        jint c = read();
        if (c < 0) return (written == 0) ? -1 : written;
        (*cbuf)[off + written] = (jchar)c;
        ++written;
    }
    return written;
}

jbool InputStreamReader::ready() {
    return (dpos_ < decoded_.size()) || (in_->available() > 0);
}

void InputStreamReader::close() {
    if (in_) in_->close();
    in_ = nullptr;
    decoded_.clear();
    dpos_ = 0;
}

} // namespace jxx::io
