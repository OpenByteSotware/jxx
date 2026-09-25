#include "ext/net/ssl/jxx.ext.net.ssl.SSLPeerUnverifiedException.h"

namespace jxx::ext::net::ssl {

SSLPeerUnverifiedException::SSLPeerUnverifiedException()
    : Super() {
}

SSLPeerUnverifiedException::SSLPeerUnverifiedException(const ::jxx::Ptr<::jxx::lang::String>& message)
    : Super(message) {
}

SSLPeerUnverifiedException::SSLPeerUnverifiedException(const char* message)
    : Super(message) {
}

::jxx::Ptr<::jxx::lang::Object> SSLPeerUnverifiedException::cloneImpl() const {
    return ::jxx::NEW<SSLPeerUnverifiedException>(*this);
}

const char* SSLPeerUnverifiedException::typeName() const noexcept {
    return "jxx.ext.net.ssl.SSLPeerUnverifiedException";
}

} // namespace jxx::ext::net::ssl
