#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslManagerBridge.h"

#include <openssl/evp.h>
#include <openssl/x509.h>

#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslX509Certificate.h"
#include "ext/net/ssl/jxx.ext.net.ssl.X509KeyManager.h"
#include "ext/net/ssl/jxx.ext.net.ssl.X509TrustManager.h"
#include "lang/jxx.lang.String.h"
#include "security/jxx.security.PrivateKey.h"

namespace jxx::ext::net::ssl::internal {
namespace {

::jxx::lang::ByteArray encodeCertificate(X509* certificate) {
    const int length = i2d_X509(certificate, nullptr);
    if (length <= 0) return nullptr;
    const auto result = ::jxx::NEW<
        ::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>(length);
    unsigned char* cursor = reinterpret_cast<unsigned char*>(&(*result)[0]);
    if (i2d_X509(certificate, &cursor) != length) return nullptr;
    return result;
}

::jxx::Ptr<::jxx::ext::net::ssl::X509TrustManager> findTrustManager(
    const std::shared_ptr<OpenSslContextConfig>& config) {
    if (config == nullptr || config->trustManagers == nullptr) return nullptr;
    for (::jxx::lang::jint index = 0; index < config->trustManagers->length; ++index) {
        const auto manager = ::jxx::CAST<::jxx::ext::net::ssl::X509TrustManager>(
            (*config->trustManagers)[index]);
        if (manager != nullptr) return manager;
    }
    return nullptr;
}

::jxx::Ptr<::jxx::ext::net::ssl::X509KeyManager> findKeyManager(
    const std::shared_ptr<OpenSslContextConfig>& config) {
    if (config == nullptr || config->keyManagers == nullptr) return nullptr;
    for (::jxx::lang::jint index = 0; index < config->keyManagers->length; ++index) {
        const auto manager = ::jxx::CAST<::jxx::ext::net::ssl::X509KeyManager>(
            (*config->keyManagers)[index]);
        if (manager != nullptr) return manager;
    }
    return nullptr;
}

} // namespace

OpenSslManagerBridge::OpenSslManagerBridge(
    const std::shared_ptr<OpenSslContextConfig>& config)
    : config_(config) {
}

int OpenSslManagerBridge::verifyPeer(X509_STORE_CTX* storeContext) noexcept {
    try {
        if (X509_verify_cert(storeContext) != 1) return 0;
        const auto manager = findTrustManager(config_);
        if (manager == nullptr) return 1;

        STACK_OF(X509)* chain = X509_STORE_CTX_get0_chain(storeContext);
        const int count = chain == nullptr ? 0 : sk_X509_num(chain);
        if (count <= 0) return 0;
        const auto converted = ::jxx::NEW<
            ::jxx::ext::net::ssl::X509TrustManager::CertificateArray>(count);
        for (int index = 0; index < count; ++index) {
            const auto encoded = encodeCertificate(sk_X509_value(chain, index));
            if (encoded == nullptr) return 0;
            (*converted)[index] = ::jxx::NEW<OpenSslX509Certificate>(encoded);
        }
        manager->checkServerTrusted(
            converted,
            ::jxx::NEW<::jxx::lang::String>("UNKNOWN"));
        X509_STORE_CTX_set_error(storeContext, X509_V_OK);
        return 1;
    } catch (...) {
        X509_STORE_CTX_set_error(storeContext, X509_V_ERR_APPLICATION_VERIFICATION);
        return 0;
    }
}

int OpenSslManagerBridge::selectClientCertificate(
    SSL* ssl,
    X509** certificate,
    EVP_PKEY** privateKey) noexcept {
    try {
        const auto manager = findKeyManager(config_);
        if (manager == nullptr) return 0;
        const auto types = ::jxx::NEW<::jxx::ext::net::ssl::X509KeyManager::StringArray>(2);
        (*types)[0] = ::jxx::NEW<::jxx::lang::String>("RSA");
        (*types)[1] = ::jxx::NEW<::jxx::lang::String>("EC");
        const auto alias = manager->chooseClientAlias(types, nullptr, nullptr);
        if (alias == nullptr) return 0;
        const auto chain = manager->getCertificateChain(alias);
        const auto key = manager->getPrivateKey(alias);
        if (chain == nullptr || chain->length == 0 || key == nullptr) return 0;

        const auto leafBytes = (*chain)[0]->getEncoded();
        const unsigned char* certCursor =
            reinterpret_cast<const unsigned char*>(&(*leafBytes)[0]);
        X509* leaf = d2i_X509(nullptr, &certCursor, leafBytes->length);
        const auto keyBytes = key->getEncoded();
        const unsigned char* keyCursor =
            reinterpret_cast<const unsigned char*>(&(*keyBytes)[0]);
        EVP_PKEY* decodedKey = d2i_AutoPrivateKey(nullptr, &keyCursor, keyBytes->length);
        if (leaf == nullptr || decodedKey == nullptr) {
            if (leaf != nullptr) X509_free(leaf);
            if (decodedKey != nullptr) EVP_PKEY_free(decodedKey);
            return 0;
        }

        for (::jxx::lang::jint index = 1; index < chain->length; ++index) {
            const auto bytes = (*chain)[index]->getEncoded();
            const unsigned char* cursor =
                reinterpret_cast<const unsigned char*>(&(*bytes)[0]);
            X509* extra = d2i_X509(nullptr, &cursor, bytes->length);
            if (extra == nullptr || SSL_add1_chain_cert(ssl, extra) != 1) {
                if (extra != nullptr) X509_free(extra);
                X509_free(leaf);
                EVP_PKEY_free(decodedKey);
                return 0;
            }
            X509_free(extra);
        }
        *certificate = leaf;
        *privateKey = decodedKey;
        return 1;
    } catch (...) {
        return 0;
    }
}

int openSslManagerBridgeExDataIndex() {
    static const int index = SSL_get_ex_new_index(0, nullptr, nullptr, nullptr, nullptr);
    return index;
}

int openSslVerifyCallback(X509_STORE_CTX* storeContext, void* argument) noexcept {
    auto* bridge = static_cast<OpenSslManagerBridge*>(argument);
    return bridge == nullptr ? 0 : bridge->verifyPeer(storeContext);
}

int openSslClientCertificateCallback(
    SSL* ssl,
    X509** certificate,
    EVP_PKEY** privateKey) noexcept {
    auto* bridge = static_cast<OpenSslManagerBridge*>(
        SSL_get_ex_data(ssl, openSslManagerBridgeExDataIndex()));
    return bridge == nullptr ? 0 :
        bridge->selectClientCertificate(ssl, certificate, privateKey);
}

} // namespace jxx::ext::net::ssl::internal
