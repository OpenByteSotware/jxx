#include "jxx.io.FilterOutputStream.h"

namespace jxx::io {

FilterOutputStream::FilterOutputStream(jxx::Ptr<OutputStream> out)
    : out_(std::move(out)) {
    if (!out_) throw jxx::lang::NullPointerException(JXX_NEW<jxx::lang::String>("out"));
}

void FilterOutputStream::write(jxx::lang::jint b) { out_->write(b); }

void FilterOutputStream::write(jxx::Ptr<ByteArray> b, jxx::lang::jint off, jxx::lang::jint len) {
    out_->write(b, off, len);
}

void FilterOutputStream::flush() { out_->flush(); }

void FilterOutputStream::close() {
    try { flush(); } catch (...) {}
    out_->close();
}

} // namespace jxx::io
