#include "util/zip/jxx.util.zip.CheckedInputStream.h"

#include <algorithm>

#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::util::zip {

CheckedInputStream::CheckedInputStream(
    const ::jxx::Ptr<::jxx::io::InputStream>& input,
    const ::jxx::Ptr<Checksum>& checksum)
    : Super(input), checksum_(checksum) {
    if (checksum_ == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
}

::jxx::lang::jint CheckedInputStream::read() {
    const auto value = in_->read();
    if (value != -1) {
        checksum_->update(value);
    }
    return value;
}

::jxx::lang::jint CheckedInputStream::read(const ::jxx::lang::ByteArray& buffer) {
    if (buffer == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    return read(buffer, 0, buffer->length);
}

::jxx::lang::jint CheckedInputStream::read(
    const ::jxx::lang::ByteArray& buffer,
    ::jxx::lang::jint offset,
    ::jxx::lang::jint length) {
    const auto count = in_->read(buffer, offset, length);
    if (count != -1) {
        checksum_->update(buffer, offset, count);
    }
    return count;
}

::jxx::lang::jlong CheckedInputStream::skip(::jxx::lang::jlong count) {
    if (count <= 0) {
        return 0;
    }

    auto buffer = ::jxx::NEW<::jxx::lang::ByteArrayType>(512);
    ::jxx::lang::jlong skipped = 0;
    while (skipped < count) {
        const auto request = static_cast<::jxx::lang::jint>(
            std::min<::jxx::lang::jlong>(count - skipped, buffer->length));
        const auto value = read(buffer, 0, request);
        if (value == -1) {
            break;
        }
        skipped += value;
    }
    return skipped;
}

::jxx::Ptr<Checksum> CheckedInputStream::getChecksum() const noexcept {
    return checksum_;
}

} // namespace jxx::util::zip
