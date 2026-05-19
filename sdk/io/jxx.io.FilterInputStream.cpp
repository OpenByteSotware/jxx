#include "jxx.io.FilterInputStream.h"

namespace jxx::io {

FilterInputStream::FilterInputStream(jxx::Ptr<InputStream> in) : in_(std::move(in)) {
    if (!in_) throw jxx::lang::NullPointerException(JXX_NEW<jxx::lang::String>("in"));
}

jint FilterInputStream::read() { return in_->read(); }

jint FilterInputStream::read(jxx::Ptr<ByteArray> b, jint off, jint len) { return in_->read(b, off, len); }

jlong FilterInputStream::skip(jlong n) { return in_->skip(n); }

jint FilterInputStream::available() { return in_->available(); }

void FilterInputStream::close() { in_->close(); }

void FilterInputStream::mark(jint rl) { in_->mark(rl); }

void FilterInputStream::reset() { in_->reset(); }

jbool FilterInputStream::markSupported() { return in_->markSupported(); }

} // namespace jxx::io
