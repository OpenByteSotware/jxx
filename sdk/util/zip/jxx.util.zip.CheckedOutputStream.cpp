#include "util/zip/jxx.util.zip.CheckedOutputStream.h"

#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::util::zip {

CheckedOutputStream::CheckedOutputStream(
    const ::jxx::Ptr<::jxx::io::OutputStream>& output,
    const ::jxx::Ptr<Checksum>& checksum)
    : Super(output), checksum_(checksum) {
    if (checksum_ == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
}

void CheckedOutputStream::write(::jxx::lang::jint value) {
    out_->write(value);
    checksum_->update(value);
}

void CheckedOutputStream::write(const ::jxx::lang::ByteArray& buffer) {
    if (buffer == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    write(buffer, 0, buffer->length);
}

void CheckedOutputStream::write(
    const ::jxx::lang::ByteArray& buffer,
    ::jxx::lang::jint offset,
    ::jxx::lang::jint length) {
    out_->write(buffer, offset, length);
    checksum_->update(buffer, offset, length);
}

::jxx::Ptr<Checksum> CheckedOutputStream::getChecksum() const noexcept {
    return checksum_;
}

} // namespace jxx::util::zip
