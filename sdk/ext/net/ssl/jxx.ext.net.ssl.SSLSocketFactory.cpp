#include "ext/net/ssl/jxx.ext.net.ssl.SSLSocketFactory.h"

namespace jxx::ext::net::ssl {

::jxx::Ptr<::jxx::ext::net::SocketFactory>
SSLSocketFactory::getDefault() {
    // Stage 1-5 establishes the Java 8 hierarchy. The OpenSSL concrete
    // factory is installed by the subsequent SSLContext/provider stage.
    return ::jxx::ext::net::SocketFactory::getDefault();
}

} // namespace jxx::ext::net::ssl
