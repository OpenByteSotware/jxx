#include <string>
#include "io/jxx.io.PrintStream.h"
#include "io/jxx.io.IOException.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

namespace jxx::io {

PrintStream::PrintStream(
    const ::jxx::Ptr<OutputStream>& output)
    : PrintStream(output, false) {
}

PrintStream::PrintStream(
    const ::jxx::Ptr<OutputStream>& output,
    ::jxx::lang::jbool autoFlush)
    : Super(output)
    , autoFlush_(autoFlush) {
}

PrintStream::~PrintStream() = default;

void PrintStream::ensureOpen_() const {
    if (closed_ || out_ == nullptr) {
        throw IOException();
    }
}

void PrintStream::setError_() noexcept {
    trouble_ = true;
}

void PrintStream::flush() {
    try {
        ensureOpen_();
        out_->flush();
    }
    catch (...) {
        setError_();
    }
}

void PrintStream::close() {
    if (closed_) {
        return;
    }

    try {
        out_->flush();
        out_->close();
    }
    catch (...) {
        setError_();
    }

    closed_ = true;
    out_.reset();
}

::jxx::lang::jbool PrintStream::checkError() {
    if (!closed_) {
        flush();
    }

    return trouble_;
}

void PrintStream::write(
    ::jxx::lang::jint value) {

    try {
        ensureOpen_();
        out_->write(value);

        if (autoFlush_ && value == '\n') {
            out_->flush();
        }
    }
    catch (...) {
        setError_();
    }
}

void PrintStream::write(
    const ::jxx::lang::ByteArray& buffer,
    ::jxx::lang::jint offset,
    ::jxx::lang::jint length) {

    try {
        ensureOpen_();
        out_->write(buffer, offset, length);

        if (autoFlush_) {
            out_->flush();
        }
    }
    catch (...) {
        setError_();
    }
}

void PrintStream::text_(
    const ::jxx::Ptr<::jxx::lang::String>& value) {

    ::jxx::Ptr<::jxx::lang::String> actual = value;

    if (actual == nullptr) {
        actual =
            ::jxx::NEW<::jxx::lang::String>("null");
    }

    const auto bytes = actual->getBytes();

    write(
        bytes,
        0,
        static_cast<::jxx::lang::jint>(
            bytes->length));
}

void PrintStream::newLine_() {
    write(static_cast<::jxx::lang::jint>('\n'));

    if (autoFlush_ && !closed_ && out_ != nullptr) {
        try {
            out_->flush();
        }
        catch (...) {
            setError_();
        }
    }
}

void PrintStream::print(
    const ::jxx::Ptr<::jxx::lang::String>& value) {
    text_(value);
}

void PrintStream::print(
    const ::jxx::Ptr<::jxx::lang::Object>& value) {

    if (value == nullptr) {
        text_(nullptr);
        return;
    }

    text_(value->toString());
}

void PrintStream::print(
    ::jxx::lang::jbool value) {

    text_(::jxx::NEW<::jxx::lang::String>(
        value ? "true" : "false"));
}

void PrintStream::print(
    ::jxx::lang::jchar value) {

    text_(::jxx::NEW<::jxx::lang::String>(
        std::u16string(1, value)));
}

void PrintStream::print(
    ::jxx::lang::jint value) {
    text_(::jxx::NEW<::jxx::lang::String>(
        std::to_string(value)));
}

void PrintStream::print(
    ::jxx::lang::jlong value) {
    text_(::jxx::NEW<::jxx::lang::String>(
        std::to_string(value)));
}

void PrintStream::print(
    ::jxx::lang::jfloat value) {
    text_(::jxx::NEW<::jxx::lang::String>(
        std::to_string(value)));
}

void PrintStream::print(
    ::jxx::lang::jdouble value) {
    text_(::jxx::NEW<::jxx::lang::String>(
        std::to_string(value)));
}

void PrintStream::println() {
    newLine_();
}

void PrintStream::println(
    const ::jxx::Ptr<::jxx::lang::String>& value) {
    print(value);
    newLine_();
}

void PrintStream::println(
    const ::jxx::Ptr<::jxx::lang::Object>& value) {
    print(value);
    newLine_();
}

void PrintStream::println(
    ::jxx::lang::jbool value) {
    print(value);
    newLine_();
}

void PrintStream::println(
    ::jxx::lang::jchar value) {
    print(value);
    newLine_();
}

void PrintStream::println(
    ::jxx::lang::jint value) {
    print(value);
    newLine_();
}

void PrintStream::println(
    ::jxx::lang::jlong value) {
    print(value);
    newLine_();
}

void PrintStream::println(
    ::jxx::lang::jfloat value) {
    print(value);
    newLine_();
}

void PrintStream::println(
    ::jxx::lang::jdouble value) {
    print(value);
    newLine_();
}

} // namespace jxx::io
