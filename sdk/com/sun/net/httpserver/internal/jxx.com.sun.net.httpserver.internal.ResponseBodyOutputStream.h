#pragma once

#include "io/jxx.io.OutputStream.h"

namespace jxx::com::sun::net::httpserver::internal {

enum class ResponseBodyMode {
    NoBody,
    FixedLength,
    Chunked
};

class ResponseBodyOutputStream final
    : public ::jxx::lang::ClassBase<
          ResponseBodyOutputStream,
          ::jxx::io::OutputStream> {
public:
    using JxxSuper = ::jxx::io::OutputStream;
    using Super = ::jxx::lang::ClassBase<ResponseBodyOutputStream, JxxSuper>;

    ResponseBodyOutputStream(
        const ::jxx::Ptr<::jxx::io::OutputStream>& delegate,
        ResponseBodyMode mode,
        ::jxx::lang::jlong expectedLength);

    void write(::jxx::lang::jint value) override;
    void write(
        const ::jxx::lang::ByteArray& buffer,
        ::jxx::lang::jint offset,
        ::jxx::lang::jint length) override;
    void flush() override;
    void close() override;

private:
    void writeAscii_(const char* bytes);

    ::jxx::Ptr<::jxx::io::OutputStream> delegate_;
    ResponseBodyMode mode_;
    ::jxx::lang::jlong expectedLength_;
    ::jxx::lang::jlong written_ = 0;
    ::jxx::lang::jbool closed_ = false;
};

} // namespace jxx::com::sun::net::httpserver::internal
