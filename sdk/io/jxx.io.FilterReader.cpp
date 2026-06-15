#include <stdexcept>
#include "io/jxx.io.FilterReader.h"


namespace
{
    void throwIAE_(const char* msg)
    {
        throw std::invalid_argument(msg);
    }
}

namespace jxx::io
{
    FilterReader::FilterReader(jxx::Ptr<Reader> in)
        : in_(std::move(in))
    {
        if (!in_)
            throwIAE_("null reader");
    }

    jxx::lang::jint FilterReader::read() { return in_->read(); }
    jxx::lang::jint FilterReader::read(const jxx::lang::CharArray cbuf, jxx::lang::jint off, jxx::lang::jint len) { return in_->read(cbuf, off, len); }
    jxx::lang::jlong FilterReader::skip(jxx::lang::jlong n) { return in_->skip(n); }
    jxx::lang::jbool FilterReader::ready() { return in_->ready(); }
    jxx::lang::jbool FilterReader::markSupported() const { return in_->markSupported(); }
    void FilterReader::mark(jxx::lang::jint readAheadLimit) { in_->mark(readAheadLimit); }
    void FilterReader::reset() { in_->reset(); }
    void FilterReader::close() { in_->close(); }
}
