#include <algorithm>
#include <string>
#include <vector>

#include <openssl/ssl.h>
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslCipherSuites.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslCompatibility.h"


#include "ext/net/ssl/jxx.ext.net.ssl.SSLException.h"
#include "lang/jxx.lang.IllegalArgumentException.h"

namespace jxx::ext::net::ssl::internal {
namespace {

using ContextMethod = const SSL_METHOD* (*)();

void configureProtocolRange(SSL_CTX* context) {
    if (SSL_CTX_set_min_proto_version(
            context,
            TLS1_2_VERSION) != 1 ||
        SSL_CTX_set_max_proto_version(
            context,
            highestSupportedTlsVersion()) != 1)
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
    bool ciphersOwned = false;

    try {
        configureProtocolRange(context);
        ssl = SSL_new(context);
        if (ssl == nullptr) {
            throw ::jxx::ext::net::ssl::SSLException(
                "Unable to create TLS state");
        }

        ciphers = supportedCipherStack(ssl, ciphersOwned);
        const auto result = toArray(ciphers);

        if (ciphersOwned) sk_SSL_CIPHER_free(ciphers);
        SSL_free(ssl);
        SSL_CTX_free(context);
        return result;
    }
    catch (...) {
        if (ciphersOwned && ciphers != nullptr) {
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

void applyEnabledCipherSuites(
    ssl_ctx_st* context,
    const std::vector<std::string>& suites) {
    if (context == nullptr)
        throw ::jxx::ext::net::ssl::SSLException(
            "TLS context is null");
    if (suites.empty())
        throw ::jxx::lang::IllegalArgumentException(
            "enabled cipher suite list is empty");

    std::vector<std::string> tls13;
    std::vector<std::string> legacy;
    for (const auto& suite : suites) {
        if (suite.empty())
            throw ::jxx::lang::IllegalArgumentException(
                "cipher suite name is empty");
        const bool modern =
            suite == "TLS_AES_128_GCM_SHA256" ||
            suite == "TLS_AES_256_GCM_SHA384" ||
            suite == "TLS_CHACHA20_POLY1305_SHA256" ||
            suite == "TLS_AES_128_CCM_SHA256" ||
            suite == "TLS_AES_128_CCM_8_SHA256";
        (modern ? tls13 : legacy).push_back(suite);
    }

    const auto join = [](const std::vector<std::string>& values) {
        std::string result;
        for (const auto& value : values) {
            if (!result.empty()) result.push_back(':');
            result += value;
        }
        return result;
    };

    if (!legacy.empty() &&
        SSL_CTX_set_cipher_list(context, join(legacy).c_str()) != 1)
        throw ::jxx::lang::IllegalArgumentException(
            "unsupported pre-TLS-1.3 cipher suite");
    if (!tls13.empty() &&
        !setTls13CipherSuites(context, join(tls13)))
        throw ::jxx::lang::IllegalArgumentException(
            "unsupported TLS 1.3 cipher suite");
}

void validateEnabledCipherSuites(
    const std::vector<std::string>& suites) {
    SSL_CTX* context = SSL_CTX_new(TLS_method());
    if (context == nullptr)
        throw ::jxx::ext::net::ssl::SSLException(
            "Unable to create TLS context");
    try {
        applyEnabledCipherSuites(context, suites);
        SSL_CTX_free(context);
    } catch (...) {
        SSL_CTX_free(context);
        throw;
    }
}

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
