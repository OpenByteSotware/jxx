#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslStreams.h"

#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslSocket.h"
#include "io/jxx.io.IOHelper.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::ext::net::ssl::internal {

OpenSslInputStream::OpenSslInputStream(OpenSslSocket* socket)
    : socket_(socket) {
    if (socket_ == nullptr) throw ::jxx::lang::NullPointerException();
}

::jxx::lang::jint OpenSslInputStream::read() {
    unsigned char value = 0U;
    return socket_->tlsRead(&value, 1) == 1
        ? static_cast<::jxx::lang::jint>(value)
        : -1;
}

::jxx::lang::jint OpenSslInputStream::read(
    const ::jxx::lang::ByteArray& buffer,
    ::jxx::lang::jint offset,
    ::jxx::lang::jint length) {
    ::jxx::io::IOHelper::checkBounds(buffer, offset, length);
    if (length == 0) return 0;
    return socket_->tlsRead(
        reinterpret_cast<unsigned char*>(&(*buffer)[offset]),
        length);
}

void OpenSslInputStream::close() {
    socket_->close();
}

OpenSslOutputStream::OpenSslOutputStream(OpenSslSocket* socket)
    : socket_(socket) {
    if (socket_ == nullptr) throw ::jxx::lang::NullPointerException();
}

void OpenSslOutputStream::write(::jxx::lang::jint value) {
    const unsigned char byte = static_cast<unsigned char>(value);
    socket_->tlsWrite(&byte, 1);
}

void OpenSslOutputStream::write(
    const ::jxx::lang::ByteArray& buffer,
    ::jxx::lang::jint offset,
    ::jxx::lang::jint length) {
    ::jxx::io::IOHelper::checkBounds(buffer, offset, length);
    if (length == 0) return;
    socket_->tlsWrite(
        reinterpret_cast<const unsigned char*>(&(*buffer)[offset]),
        length);
}

void OpenSslOutputStream::close() {
    socket_->close();
}

} // namespace jxx::ext::net::ssl::internal
