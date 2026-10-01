#pragma once

#include "io/jxx.io.InputStream.h"
#include "io/jxx.io.OutputStream.h"

namespace jxx::ext::net::ssl::internal {

class OpenSslSocket;

class OpenSslInputStream final
    : public ::jxx::lang::ClassBase<
          OpenSslInputStream,
          ::jxx::io::InputStream> {
public:
    explicit OpenSslInputStream(OpenSslSocket* socket);

    ::jxx::lang::jint read() override;
    ::jxx::lang::jint read(
        const ::jxx::lang::ByteArray& buffer,
        ::jxx::lang::jint offset,
        ::jxx::lang::jint length) override;
    void close() override;

private:
    OpenSslSocket* socket_;
};

class OpenSslOutputStream final
    : public ::jxx::lang::ClassBase<
          OpenSslOutputStream,
          ::jxx::io::OutputStream> {
public:
    explicit OpenSslOutputStream(OpenSslSocket* socket);

    void write(::jxx::lang::jint value) override;
    void write(
        const ::jxx::lang::ByteArray& buffer,
        ::jxx::lang::jint offset,
        ::jxx::lang::jint length) override;
    void close() override;

private:
    OpenSslSocket* socket_;
};

} // namespace jxx::ext::net::ssl::internal
