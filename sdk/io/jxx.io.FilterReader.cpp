#include "io/jxx.io.FilterReader.h"

#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::io {

FilterReader::FilterReader(
    const ::jxx::Ptr<Reader>& input)
    : Super(input)
    , in_(input) {

    if (input == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
}

::jxx::lang::jint FilterReader::read() {
    return lock->synchronized([&]() -> ::jxx::lang::jint {
        return in_->read();
    });
}

::jxx::lang::jint FilterReader::read(
    const ::jxx::lang::CharArray& buffer,
    ::jxx::lang::jint offset,
    ::jxx::lang::jint length) {

    return lock->synchronized([&]() -> ::jxx::lang::jint {
        return in_->read(buffer, offset, length);
    });
}

::jxx::lang::jlong FilterReader::skip(
    ::jxx::lang::jlong count) {

    return lock->synchronized([&]() -> ::jxx::lang::jlong {
        return in_->skip(count);
    });
}

::jxx::lang::jbool FilterReader::ready() {
    return lock->synchronized([&]() -> ::jxx::lang::jbool {
        return in_->ready();
    });
}

::jxx::lang::jbool FilterReader::markSupported() const {
    return in_->markSupported();
}

void FilterReader::mark(
    ::jxx::lang::jint readAheadLimit) {

    lock->synchronized([&] {
        in_->mark(readAheadLimit);
    });
}

void FilterReader::reset() {
    lock->synchronized([&] {
        in_->reset();
    });
}

void FilterReader::close() {
    lock->synchronized([&] {
        in_->close();
    });
}

} // namespace jxx::io
