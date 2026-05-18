#include "jxx.io.FilterInputStream.h"

namespace jxx::io {

    FilterInputStream::FilterInputStream(jxx::Ptr<InputStream> in)
        : in_(std::move(in)) {}

    jxx::lang::jint FilterInputStream::read() { return in_->read(); }
    jxx::lang::jint FilterInputStream::read(jxx::Ptr<ByteArray> b, jxx::lang::jint off, jxx::lang::jint len) {
        return in_->read(b, off, len);
    }

} // namespace jxx::io