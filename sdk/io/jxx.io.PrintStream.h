#pragma once

#include "io/jxx.io.FilterOutputStream.h"
#include "io/jxx.io.Closeable.h"
#include "lang/jxx.lang.Appendable.h"
#include "lang/jxx_types.h"
#include "lang/jxx.lang.JxxArray.h"

namespace jxx::lang {
class Object;
class String;
}

namespace jxx::util { class Locale; }

namespace jxx::io {

class PrintStream final
    : public ::jxx::lang::ClassBase<
          PrintStream,
          FilterOutputStream,
          ::jxx::lang::Appendable,
          Closeable> {
public:
    using JxxSuper = FilterOutputStream;

    using Super =
        ::jxx::lang::ClassBase<
            PrintStream,
            JxxSuper,
            ::jxx::lang::Appendable,
            Closeable>;

    explicit PrintStream(
        const ::jxx::Ptr<OutputStream>& output);

    PrintStream(
        const ::jxx::Ptr<OutputStream>& output,
        ::jxx::lang::jbool autoFlush);

    ~PrintStream() override;

    void flush() override;
    void close() override;

    ::jxx::lang::jbool checkError();

    ::jxx::Ptr<::jxx::lang::Appendable> append(
        const ::jxx::Ptr<::jxx::lang::CharSequence>& sequence) override;

    ::jxx::Ptr<::jxx::lang::Appendable> append(
        const ::jxx::Ptr<::jxx::lang::CharSequence>& sequence,
        ::jxx::lang::jint start,
        ::jxx::lang::jint end) override;

    ::jxx::Ptr<::jxx::lang::Appendable> append(
        ::jxx::lang::jchar value) override;

    void write(
        ::jxx::lang::jint value) override;

    void write(
        const ::jxx::lang::ByteArray& buffer,
        ::jxx::lang::jint offset,
        ::jxx::lang::jint length) override;

    void print(
        const ::jxx::Ptr<::jxx::lang::String>& value);

    void print(
        const ::jxx::Ptr<::jxx::lang::Object>& value);

    void print(::jxx::lang::jbool value);
    void print(::jxx::lang::jchar value);
    void print(::jxx::lang::jint value);
    void print(::jxx::lang::jlong value);
    void print(::jxx::lang::jfloat value);
    void print(::jxx::lang::jdouble value);

    void println();

    void println(
        const ::jxx::Ptr<::jxx::lang::String>& value);

    void println(
        const ::jxx::Ptr<::jxx::lang::Object>& value);

    void println(::jxx::lang::jbool value);
    void println(::jxx::lang::jchar value);
    void println(::jxx::lang::jint value);
    void println(::jxx::lang::jlong value);
    void println(::jxx::lang::jfloat value);
    void println(::jxx::lang::jdouble value);

    ::jxx::Ptr<PrintStream> printf(
        const ::jxx::Ptr<::jxx::lang::String>& format,
        const ::jxx::Ptr<::jxx::lang::JxxArray<
            ::jxx::Ptr<::jxx::lang::Object>, 1U>>& args);

    ::jxx::Ptr<PrintStream> printf(
        const ::jxx::Ptr<::jxx::util::Locale>& locale,
        const ::jxx::Ptr<::jxx::lang::String>& format,
        const ::jxx::Ptr<::jxx::lang::JxxArray<
            ::jxx::Ptr<::jxx::lang::Object>, 1U>>& args);

    ::jxx::Ptr<PrintStream> format(
        const ::jxx::Ptr<::jxx::lang::String>& format,
        const ::jxx::Ptr<::jxx::lang::JxxArray<
            ::jxx::Ptr<::jxx::lang::Object>, 1U>>& args);

    ::jxx::Ptr<PrintStream> format(
        const ::jxx::Ptr<::jxx::util::Locale>& locale,
        const ::jxx::Ptr<::jxx::lang::String>& format,
        const ::jxx::Ptr<::jxx::lang::JxxArray<
            ::jxx::Ptr<::jxx::lang::Object>, 1U>>& args);

private:
    void ensureOpen_() const;

    void text_(
        const ::jxx::Ptr<::jxx::lang::String>& value);

    void newLine_();
    void setError_() noexcept;

    ::jxx::lang::jbool autoFlush_ = false;
    ::jxx::lang::jbool trouble_ = false;
    ::jxx::lang::jbool closed_ = false;
};

} // namespace jxx::io
