#pragma once
#include <openssl/ssl.h>
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslSession.h"
#include "lang/jxx.lang.String.h"
namespace jxx::ext::net::ssl::internal {
void configureHttpsEndpointIdentification(SSL* ssl,const ::jxx::Ptr<::jxx::lang::String>& peerHost);
::jxx::Ptr<OpenSslSession::CertificateArray> peerCertificateChain(SSL* ssl);
}
