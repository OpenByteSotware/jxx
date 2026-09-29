#include <openssl/ssl.h>
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslCipherSuites.h"


#include "ext/net/ssl/jxx.ext.net.ssl.SSLException.h"

namespace jxx::ext::net::ssl::internal {
namespace {

using ContextMethod = const SSL_METHOD* (*)();

void configureProtocolRange(SSL_CTX* context) {
    if (SSL_CTX_set_min_proto_version(
            context,
            TLS1_2_VERSION) != 1 ||
        SSL_CTX_set_max_proto_version(
            context,
            TLS1_3_VERSION) != 1)
    {
        throw ::jxx::ext::net::ssl::SSLException(
            "Unable to configure TLS protocol range");
    }
}

::jxx::Ptr<CipherSuiteArray>
toArray(STACK_OF(SSL_CIPHER)* ciphers) {
    const int count =
        ciphers == nullptr
            ? 0
            : sk_SSL_CIPHER_num(ciphers);

    const auto result =
        ::jxx::NEW<CipherSuiteArray>(count);

    for (int index = 0; index < count; ++index) {
        const SSL_CIPHER* cipher =
            sk_SSL_CIPHER_value(
                ciphers,
                index);

        const char* name =
            cipher == nullptr
                ? nullptr
                : SSL_CIPHER_get_name(cipher);

        (*result)[index] =
            ::jxx::NEW<::jxx::lang::String>(
                name == nullptr ? "" : name);
    }

    return result;
}

::jxx::Ptr<CipherSuiteArray>
defaultCipherSuites(ContextMethod method) {
    SSL_CTX* context = SSL_CTX_new(method());
    if (context == nullptr) {
        throw ::jxx::ext::net::ssl::SSLException(
            "Unable to create TLS context");
    }

    try {
        configureProtocolRange(context);
        const auto result =
            toArray(SSL_CTX_get_ciphers(context));
        SSL_CTX_free(context);
        return result;
    }
    catch (...) {
        SSL_CTX_free(context);
        throw;
    }
}

::jxx::Ptr<CipherSuiteArray>
supportedCipherSuites(ContextMethod method) {
    SSL_CTX* context = SSL_CTX_new(method());
    if (context == nullptr) {
        throw ::jxx::ext::net::ssl::SSLException(
            "Unable to create TLS context");
    }

    SSL* ssl = nullptr;
    STACK_OF(SSL_CIPHER)* ciphers = nullptr;

    try {
        configureProtocolRange(context);
        ssl = SSL_new(context);
        if (ssl == nullptr) {
            throw ::jxx::ext::net::ssl::SSLException(
                "Unable to create TLS state");
        }

        ciphers = SSL_get1_supported_ciphers(ssl);
        const auto result = toArray(ciphers);

        sk_SSL_CIPHER_free(ciphers);
        SSL_free(ssl);
        SSL_CTX_free(context);
        return result;
    }
    catch (...) {
        if (ciphers != nullptr) {
            sk_SSL_CIPHER_free(ciphers);
        }
        if (ssl != nullptr) {
            SSL_free(ssl);
        }
        SSL_CTX_free(context);
        throw;
    }
}

} // namespace

::jxx::Ptr<CipherSuiteArray>
clientDefaultCipherSuites() {
    return defaultCipherSuites(TLS_client_method);
}

::jxx::Ptr<CipherSuiteArray>
clientSupportedCipherSuites() {
    return supportedCipherSuites(TLS_client_method);
}

::jxx::Ptr<CipherSuiteArray>
serverDefaultCipherSuites() {
    return defaultCipherSuites(TLS_server_method);
}

::jxx::Ptr<CipherSuiteArray>
serverSupportedCipherSuites() {
    return supportedCipherSuites(TLS_server_method);
}

} // namespace jxx::ext::net::ssl::internal
