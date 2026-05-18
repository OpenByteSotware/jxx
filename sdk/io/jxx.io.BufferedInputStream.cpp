#include "jxx.io.BufferedInputStream.h"

namespace jxx::io {

    BufferedInputStream::BufferedInputStream(jxx::Ptr<InputStream> in, jxx::lang::jint size)
        : FilterInputStream(std::move(in)) {
        buf_ = JXX_NEW<ByteArray>((std::uint32_t)size);
    }

    jxx::lang::jint BufferedInputStream::read() {
        if (pos_ >= count_) {
            count_ = in_->read(buf_, 0, (jxx::lang::jint)buf_->length);
            pos_ = 0;
            if (count_ <= 0) return -1;
        }
        return ((jxx::lang::jint)(*buf_)[pos_++]) & 0xFF;
    }

} // namespace jxx::io