#include "ext/net/ssl/jxx.ext.net.ssl.SSLKeyException.h"

namespace jxx::ext::net::ssl {

SSLKeyException::SSLKeyException()
    : Super() {
}

SSLKeyException::SSLKeyException(const ::jxx::Ptr<::jxx::lang::String>& message)
    : Super(message) {
}

SSLKeyException::SSLKeyException(const char* message)
    : Super(message) {
}

::jxx::Ptr<::jxx::lang::Object> SSLKeyException::cloneImpl() const {
    return ::jxx::NEW<SSLKeyException>(*this);
}

const char* SSLKeyException::typeName() const noexcept {
    return "jxx.ext.net.ssl.SSLKeyException";
}

} // namespace jxx::ext::net::ssl
