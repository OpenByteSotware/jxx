#pragma once
#include <openssl/ssl.h>
namespace jxx::ext::net::ssl::internal {
class OpenSslSessionContext;
void configureServerSessionCache(
    SSL_CTX* context,
    OpenSslSessionContext* sessionContext);
} // namespace jxx::ext::net::ssl::internal
