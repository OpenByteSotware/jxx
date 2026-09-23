#include "io/jxx.io.OutputStreamWriter.h"

#include <string>

#include "io/jxx.io.IOException.h"
#include "io/jxx.io.OutputStream.h"
#include "io/jxx.io.UnsupportedEncodingException.h"
#include "nio/charset/jxx.nio.charset.Charset.h"
#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"

namespace jxx::io {

OutputStreamWriter::OutputStreamWriter(
    const ::jxx::Ptr<OutputStream>& output)
    : OutputStreamWriter(
          output,
          ::jxx::NEW<::jxx::lang::String>("UTF-8")) {
}

OutputStreamWriter::OutputStreamWriter(
    const ::jxx::Ptr<OutputStream>& output,
    const ::jxx::Ptr<::jxx::lang::String>& charsetName)
    : Super()
    , out_(output)
    , encoding_(charsetName) {

    if (output == nullptr || charsetName == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    if (!::jxx::nio::charset::Charset::isSupported(charsetName)) {
        throw UnsupportedEncodingException(charsetName->utf8());
    }
    encoding_ = ::jxx::nio::charset::Charset::forName(charsetName)->name();
}

void OutputStreamWriter::writeString(
    const ::jxx::Ptr<::jxx::lang::String>& value) {
    if (out_ == nullptr) throw IOException();
    const auto encoding = encoding_->utf8();
    if (encoding == "ISO-8859-1" || encoding == "US-ASCII") {
        auto bytes = std::make_shared<::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>(
            static_cast<std::uint32_t>(value->length()));
        for (::jxx::lang::jint index = 0; index < value->length(); ++index) {
            const auto ch = value->charAt(index);
            (*bytes)[index] = static_cast<::jxx::lang::jbyte>(
                encoding == "US-ASCII" && ch > 0x7F ? '?' : (ch <= 0xFF ? ch : '?'));
        }
        out_->write(bytes, 0, static_cast<::jxx::lang::jint>(bytes->length));
    } else {
        const auto bytes = value->getBytes();
        out_->write(bytes, 0, static_cast<::jxx::lang::jint>(bytes->length));
    }
}

void OutputStreamWriter::write(::jxx::lang::jint value) {
    lock->synchronized([&] {
        const std::u16string text(
            1,
            static_cast<char16_t>(value));
        writeString(::jxx::NEW<::jxx::lang::String>(text));
    });
}

void OutputStreamWriter::write(
    const ::jxx::lang::CharArray& buffer,
    ::jxx::lang::jint offset,
    ::jxx::lang::jint length) {

    lock->synchronized([&] {
        if (buffer == nullptr) {
            throw ::jxx::lang::NullPointerException();
        }
        if (offset < 0 || length < 0 ||
            offset > static_cast<::jxx::lang::jint>(buffer->length) - length) {
            throw ::jxx::lang::IndexOutOfBoundsException();
        }
        if (out_ == nullptr) {
            throw IOException();
        }

        std::u16string text;
        text.reserve(static_cast<std::size_t>(length));
        for (::jxx::lang::jint index = 0;
             index < length;
             ++index) {
            text.push_back((*buffer)[offset + index]);
        }
        writeString(::jxx::NEW<::jxx::lang::String>(text));
    });
}

void OutputStreamWriter::write(
    const ::jxx::Ptr<::jxx::lang::String>& value,
    ::jxx::lang::jint offset,
    ::jxx::lang::jint length) {

    lock->synchronized([&] {
        if (value == nullptr) {
            throw ::jxx::lang::NullPointerException();
        }
        if (offset < 0 || length < 0 ||
            offset > value->length() - length) {
            throw ::jxx::lang::IndexOutOfBoundsException();
        }
        if (out_ == nullptr) {
            throw IOException();
        }
        writeString(value->substring(offset, offset + length));
    });
}

void OutputStreamWriter::flush() {
    lock->synchronized([&] {
        if (out_ == nullptr) {
            throw IOException();
        }
        out_->flush();
    });
}

void OutputStreamWriter::close() {
    lock->synchronized([&] {
        if (out_ == nullptr) {
            return;
        }
        out_->close();
        out_.reset();
    });
}

::jxx::Ptr<::jxx::lang::String>
OutputStreamWriter::getEncoding() const {
    return lock->synchronized(
        [&]() -> ::jxx::Ptr<::jxx::lang::String> {
            return out_ != nullptr ? encoding_ : nullptr;
        });
}

} // namespace jxx::io
