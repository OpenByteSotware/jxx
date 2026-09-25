#include "ext/net/ssl/jxx.ext.net.ssl.SSLException.h"

namespace jxx::ext::net::ssl {

SSLException::SSLException()
    : Super() {
}

SSLException::SSLException(const ::jxx::Ptr<::jxx::lang::String>& message)
    : Super(message) {
}

SSLException::SSLException(const char* message)
    : Super(message) {
}

::jxx::Ptr<::jxx::lang::Object> SSLException::cloneImpl() const {
    return ::jxx::NEW<SSLException>(*this);
}

const char* SSLException::typeName() const noexcept {
    return "jxx.ext.net.ssl.SSLException";
}

} // namespace jxx::ext::net::ssl
