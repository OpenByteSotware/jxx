#include "lang/jxx.lang.NullPointerException.h"
#include "jxx.io.FilterInputStream.h"

namespace jxx::io {

FilterInputStream::FilterInputStream(const jxx::Ptr<InputStream> in) : in_(std::move(in)) {
    if (!in_) throw jxx::lang::NullPointerException(jxx::NEW<jxx::lang::String>("in"));
}

jxx::lang::jint FilterInputStream::read() { return in_->read(); }

jxx::lang::jint FilterInputStream::read(const jxx::lang::ByteArray b, jxx::lang::jint off, jxx::lang::jint len) { return in_->read(b, off, len); }

jxx::lang::jlong FilterInputStream::skip(jxx::lang::jlong n) { return in_->skip(n); }

jxx::lang::jint FilterInputStream::available() { return in_->available(); }
void FilterInputStream::close() { in_->close(); }

void FilterInputStream::mark(jxx::lang::jint rl) { in_->mark(rl); }

void FilterInputStream::reset() { in_->reset(); }

jxx::lang::jbool FilterInputStream::markSupported() { return in_->markSupported(); }
} // namespace jxx::io
