#pragma once

#include <openssl/ssl.h>

#include "ext/net/ssl/jxx.ext.net.ssl.SSLSession.h"

namespace jxx::ext::net::ssl::internal {

::jxx::Ptr<::jxx::ext::net::ssl::SSLSession::StringArray>
localSupportedSignatureAlgorithms(SSL* ssl);

::jxx::Ptr<::jxx::ext::net::ssl::SSLSession::StringArray>
peerSupportedSignatureAlgorithms(SSL* ssl);

} // namespace jxx::ext::net::ssl::internal
