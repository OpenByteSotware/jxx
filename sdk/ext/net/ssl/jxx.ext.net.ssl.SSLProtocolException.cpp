#include "ext/net/ssl/jxx.ext.net.ssl.SSLProtocolException.h"

namespace jxx::ext::net::ssl {

SSLProtocolException::SSLProtocolException()
    : Super() {
}

SSLProtocolException::SSLProtocolException(const ::jxx::Ptr<::jxx::lang::String>& message)
    : Super(message) {
}

SSLProtocolException::SSLProtocolException(const char* message)
    : Super(message) {
}

::jxx::Ptr<::jxx::lang::Object> SSLProtocolException::cloneImpl() const {
    return ::jxx::NEW<SSLProtocolException>(*this);
}

const char* SSLProtocolException::typeName() const noexcept {
    return "jxx.ext.net.ssl.SSLProtocolException";
}

} // namespace jxx::ext::net::ssl
