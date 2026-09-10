#include "io/jxx.io.Writer.h"

#include "lang/jxx.lang.CharSequence.h"
#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"

namespace jxx::io {

namespace {

void checkBounds(
    const ::jxx::lang::CharArray& buffer,
    ::jxx::lang::jint offset,
    ::jxx::lang::jint length) {

    if (buffer == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }

    if (offset < 0 ||
        length < 0 ||
        offset > static_cast<::jxx::lang::jint>(buffer->length) - length) {
        throw ::jxx::lang::IndexOutOfBoundsException();
    }
}

void checkSequenceBounds(
    ::jxx::lang::jint sequenceLength,
    ::jxx::lang::jint start,
    ::jxx::lang::jint end) {

    if (start < 0 ||
        end < start ||
        end > sequenceLength) {
        throw ::jxx::lang::IndexOutOfBoundsException();
    }
}

} // namespace

Writer::Writer()
    : lock_(thisPtr()) {
}

Writer::Writer(
    const ::jxx::Ptr<::jxx::lang::Object>& lock)
    : lock_(lock) {

    if (lock_ == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
}

void Writer::write(
    ::jxx::lang::jint value) {

    auto buffer =
        ::jxx::NEW<::jxx::lang::CharArrayType>(1);

    (*buffer)[0] =
        static_cast<::jxx::lang::jchar>(value);

    write(buffer, 0, 1);
}

void Writer::write(
    const ::jxx::lang::CharArray& buffer) {

    const auto length =
        static_cast<::jxx::lang::jint>(buffer == nullptr ? 0 : buffer->length);

    checkBounds(buffer, 0, length);
    write(buffer, 0, length);
}

void Writer::write(
    const ::jxx::Ptr<::jxx::lang::String>& value) {

    if (value == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }

    write(value, 0, value->length());
}

void Writer::write(
    const ::jxx::Ptr<::jxx::lang::String>& value,
    ::jxx::lang::jint offset,
    ::jxx::lang::jint length) {

    if (value == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }

    if (offset < 0 ||
        length < 0 ||
        offset > value->length() - length) {
        throw ::jxx::lang::IndexOutOfBoundsException();
    }

    const auto buffer = value->toCharArray();
    write(buffer, offset, length);
}

::jxx::Ptr<Writer> Writer::append(
    const ::jxx::Ptr<::jxx::lang::CharSequence>& sequence) {

    if (sequence == nullptr) {
        write(::jxx::NEW<::jxx::lang::String>("null"));
        return ::jxx::CAST<Writer>(thisPtr());
    }

    const ::jxx::lang::jint length =
        sequence->length();

    for (::jxx::lang::jint index = 0;
         index < length;
         ++index) {
        write(static_cast<::jxx::lang::jint>(
            sequence->charAt(index)));
    }

    return ::jxx::CAST<Writer>(thisPtr());
}

::jxx::Ptr<Writer> Writer::append(
    const ::jxx::Ptr<::jxx::lang::CharSequence>& sequence,
    ::jxx::lang::jint start,
    ::jxx::lang::jint end) {

    if (sequence == nullptr) {
        const auto nullText =
            ::jxx::NEW<::jxx::lang::String>("null");

        checkSequenceBounds(
            nullText->length(),
            start,
            end);

        for (::jxx::lang::jint index = start;
             index < end;
             ++index) {
            write(static_cast<::jxx::lang::jint>(
                nullText->charAt(index)));
        }

        return ::jxx::CAST<Writer>(thisPtr());
    }

    checkSequenceBounds(
        sequence->length(),
        start,
        end);

    for (::jxx::lang::jint index = start;
         index < end;
         ++index) {
        write(static_cast<::jxx::lang::jint>(
            sequence->charAt(index)));
    }

    return ::jxx::CAST<Writer>(thisPtr());
}

::jxx::Ptr<Writer> Writer::append(
    ::jxx::lang::jchar value) {

    write(static_cast<::jxx::lang::jint>(value));
    return ::jxx::CAST<Writer>(thisPtr());
}

} // namespace jxx::io
