#include "ext/net/ssl/jxx.ext.net.ssl.SSLHandshakeException.h"

namespace jxx::ext::net::ssl {

SSLHandshakeException::SSLHandshakeException()
    : Super() {
}

SSLHandshakeException::SSLHandshakeException(const ::jxx::Ptr<::jxx::lang::String>& message)
    : Super(message) {
}

SSLHandshakeException::SSLHandshakeException(const char* message)
    : Super(message) {
}

::jxx::Ptr<::jxx::lang::Object> SSLHandshakeException::cloneImpl() const {
    return ::jxx::NEW<SSLHandshakeException>(*this);
}

const char* SSLHandshakeException::typeName() const noexcept {
    return "jxx.ext.net.ssl.SSLHandshakeException";
}

} // namespace jxx::ext::net::ssl
