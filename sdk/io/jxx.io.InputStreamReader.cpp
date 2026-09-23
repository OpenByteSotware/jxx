#include <algorithm>
#include "io/jxx.io.InputStreamReader.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.IOException.h"
#include "io/jxx.io.InputStream.h"
#include "io/jxx.io.UnsupportedEncodingException.h"
#include "nio/charset/jxx.nio.charset.Charset.h"
#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"

namespace jxx::io {

InputStreamReader::InputStreamReader(
    const ::jxx::Ptr<InputStream>& input)
    : InputStreamReader(
          input,
          ::jxx::NEW<::jxx::lang::String>("UTF-8")) {
}

InputStreamReader::InputStreamReader(
    const ::jxx::Ptr<InputStream>& input,
    const ::jxx::Ptr<::jxx::lang::String>& charsetName)
    : Super()
    , in_(input)
    , encoding_(charsetName) {

    if (input == nullptr || charsetName == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    if (!::jxx::nio::charset::Charset::isSupported(charsetName)) {
        throw UnsupportedEncodingException(charsetName->utf8());
    }
    encoding_ = ::jxx::nio::charset::Charset::forName(charsetName)->name();
}

void InputStreamReader::decodeAll() {
    if (decoded_ != nullptr) {
        return;
    }
    if (in_ == nullptr) {
        throw IOException();
    }

    const auto output = ::jxx::NEW<ByteArrayOutputStream>();
    for (;;) {
        const auto value = in_->read();
        if (value < 0) {
            break;
        }
        output->write(value);
    }
    const auto bytes = output->toByteArray();
    const auto encoding = encoding_->utf8();
    if (encoding == "ISO-8859-1" || encoding == "US-ASCII") {
        std::u16string decoded;
        decoded.reserve(bytes->length);
        for (std::uint32_t index = 0; index < bytes->length; ++index) {
            const auto value = static_cast<unsigned char>((*bytes)[index]);
            decoded.push_back(static_cast<char16_t>(
                encoding == "US-ASCII" && value > 0x7FU ? 0xFFFDU : value));
        }
        decoded_ = ::jxx::NEW<::jxx::lang::String>(decoded);
    } else {
        decoded_ = ::jxx::NEW<::jxx::lang::String>(bytes);
    }
}

::jxx::lang::jint InputStreamReader::read() {
    return lock->synchronized([&]() -> ::jxx::lang::jint {
        decodeAll();
        return position_ >= decoded_->length()
            ? -1
            : static_cast<::jxx::lang::jint>(
                  decoded_->charAt(position_++));
    });
}

::jxx::lang::jint InputStreamReader::read(
    const ::jxx::lang::CharArray& buffer,
    ::jxx::lang::jint offset,
    ::jxx::lang::jint length) {

    return lock->synchronized([&]() -> ::jxx::lang::jint {
        if (buffer == nullptr) {
            throw ::jxx::lang::NullPointerException();
        }
        if (offset < 0 || length < 0 ||
            offset > static_cast<::jxx::lang::jint>(buffer->length) - length) {
            throw ::jxx::lang::IndexOutOfBoundsException();
        }
        if (length == 0) {
            return 0;
        }

        decodeAll();
        if (position_ >= decoded_->length()) {
            return -1;
        }

        const auto count = std::min(
            length,
            decoded_->length() - position_);
        for (::jxx::lang::jint index = 0;
             index < count;
             ++index) {
            (*buffer)[offset + index] =
                decoded_->charAt(position_ + index);
        }
        position_ += count;
        return count;
    });
}

::jxx::lang::jbool InputStreamReader::ready() {
    return lock->synchronized([&]() -> ::jxx::lang::jbool {
        if (in_ == nullptr) {
            throw IOException();
        }
        return decoded_ != nullptr || in_->available() > 0;
    });
}

void InputStreamReader::close() {
    lock->synchronized([&] {
        if (in_ == nullptr) {
            return;
        }
        in_->close();
        in_.reset();
        decoded_.reset();
    });
}

::jxx::Ptr<::jxx::lang::String>
InputStreamReader::getEncoding() const {
    return lock->synchronized(
        [&]() -> ::jxx::Ptr<::jxx::lang::String> {
            return in_ != nullptr ? encoding_ : nullptr;
        });
}

} // namespace jxx::io
