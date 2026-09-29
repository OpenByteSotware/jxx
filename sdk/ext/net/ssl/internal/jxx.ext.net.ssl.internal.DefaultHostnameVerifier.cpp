#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.DefaultHostnameVerifier.h"

#include <openssl/x509.h>
#include <openssl/x509v3.h>

#include "security/cert/jxx.security.cert.Certificate.h"

namespace jxx::ext::net::ssl::internal {

::jxx::lang::jbool DefaultHostnameVerifier::verify(
    const ::jxx::Ptr<::jxx::lang::String>& hostname,
    const ::jxx::Ptr<::jxx::ext::net::ssl::SSLSession>& session) {
    if (hostname == nullptr || hostname->utf8().empty() || session == nullptr)
        return false;

    const auto certificates = session->getPeerCertificates();
    if (certificates == nullptr || certificates->length == 0 ||
        (*certificates)[0] == nullptr)
        return false;

    const auto encoded = (*certificates)[0]->getEncoded();
    if (encoded == nullptr || encoded->length == 0) return false;

    const unsigned char* cursor =
        reinterpret_cast<const unsigned char*>(&(*encoded)[0]);
    X509* certificate = d2i_X509(nullptr, &cursor, encoded->length);
    if (certificate == nullptr) return false;

    const auto host = hostname->utf8();
    const int ipResult = X509_check_ip_asc(certificate, host.c_str(), 0U);
    const int hostResult = ipResult == 0
        ? X509_check_host(
              certificate,
              host.c_str(),
              host.size(),
              X509_CHECK_FLAG_NO_PARTIAL_WILDCARDS,
              nullptr)
        : ipResult;

    X509_free(certificate);
    return hostResult == 1;
}

} // namespace jxx::ext::net::ssl::internal
