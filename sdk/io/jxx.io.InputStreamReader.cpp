#include "lang/jxx.lang.Charset.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "jxx.io.IOHelper.h"
#include "jxx.io.InputStreamReader.h"

namespace jxx::io {

InputStreamReader::InputStreamReader(const jxx::Ptr<InputStream> in)
    : InputStreamReader(std::move(in), jxx::lang::Charset::defaultCharset()) {}

InputStreamReader::InputStreamReader(const jxx::Ptr<InputStream> in, const jxx::Ptr<jxx::lang::Charset> cs)
    : in_(std::move(in)), cs_(std::move(cs)) {
    if (!in_) throw jxx::lang::NullPointerException(jxx::NEW<jxx::lang::String>("in"));
    if (!cs_) throw jxx::lang::NullPointerException(jxx::NEW<jxx::lang::String>("charset"));
}

jxx::lang::jbool InputStreamReader::refill_() {
    decoded_.clear();
    dpos_ = 0;

    auto buf = jxx::NEW<jxx::lang::ByteArrayType>(4096);
    jxx::lang::jint r = in_->read(buf, 0, (jxx::lang::jint)buf->length);
    if (r < 0) return false;

    auto slice = jxx::NEW<jxx::lang::ByteArrayType>((std::uint32_t)r);
    for (jxx::lang::jint i = 0; i < r; ++i) (*slice)[i] = (*buf)[i];
    auto s = cs_->decode(slice);
    decoded_ = s ? s->utf16() : std::u16string{};
    return (r > 0);
}

jxx::lang::jint InputStreamReader::read() {
    if (dpos_ >= decoded_.size()) {
        if (!refill_()) return -1;
        if (decoded_.empty()) return -1;
    }
    return (jxx::lang::jint)decoded_[dpos_++];
}

jxx::lang::jint InputStreamReader::read(const jxx::lang::CharArray cbuf, jxx::lang::jint off, jxx::lang::jint len) {
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

jxx::lang::jbool InputStreamReader::ready() {
    return (dpos_ < decoded_.size()) || (in_->available() > 0);
}

void InputStreamReader::close() {
    if (in_) in_->close();
    in_ = nullptr;
    decoded_.clear();
    dpos_ = 0;
}

} // namespace jxx::io
