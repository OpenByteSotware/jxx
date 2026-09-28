#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslManagerBridge.h"

#include <openssl/evp.h>
#include <openssl/x509.h>

#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslX509Certificate.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslPublicKey.h"
#include "util/jxx.util.HashSet.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLEngine.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLSocket.h"
#include "ext/net/ssl/jxx.ext.net.ssl.X509ExtendedKeyManager.h"
#include "ext/net/ssl/jxx.ext.net.ssl.X509ExtendedTrustManager.h"
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

::jxx::lang::ByteArray encodePublicKey(EVP_PKEY* key) {
    const int length = i2d_PUBKEY(key, nullptr);
    if (length <= 0) return nullptr;
    const auto result = ::jxx::NEW<
        ::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>(length);
    unsigned char* cursor = reinterpret_cast<unsigned char*>(&(*result)[0]);
    return i2d_PUBKEY(key, &cursor) == length ? result : nullptr;
}

::jxx::Ptr<::jxx::lang::String> publicKeyAlgorithm(EVP_PKEY* key) {
    const int id = EVP_PKEY_base_id(key);
    const char* name = OBJ_nid2sn(id);
    return ::jxx::NEW<::jxx::lang::String>(name == nullptr ? "UNKNOWN" : name);
}

::jxx::Ptr<::jxx::util::HashSet<::jxx::security::CryptoPrimitive>>
primitiveSet(const ::jxx::Ptr<::jxx::security::CryptoPrimitive>& primitive) {
    const auto result = ::jxx::NEW<
        ::jxx::util::HashSet<::jxx::security::CryptoPrimitive>>();
    result->add(primitive);
    return result;
}

::jxx::lang::jbool permitsLocalIdentity(
    const ::jxx::Ptr<::jxx::security::AlgorithmConstraints>& constraints,
    X509* certificate,
    const ::jxx::Ptr<::jxx::security::PrivateKey>& privateKey) {
    if (constraints == nullptr) return true;
    if (certificate == nullptr || privateKey == nullptr) return false;

    const auto signaturePrimitives = primitiveSet(
        ::jxx::security::CryptoPrimitive::SIGNATURE());
    const auto keyAgreementPrimitives = primitiveSet(
        ::jxx::security::CryptoPrimitive::KEY_AGREEMENT());

    const int signatureNid = X509_get_signature_nid(certificate);
    const char* signatureName = OBJ_nid2sn(signatureNid);
    const auto signatureAlgorithm = ::jxx::NEW<::jxx::lang::String>(
        signatureName == nullptr ? "UNKNOWN" : signatureName);

    if (!constraints->permits(
            signaturePrimitives,
            signatureAlgorithm,
            nullptr))
        return false;

    const auto key = ::jxx::CAST<::jxx::security::Key>(privateKey);
    if (key == nullptr || !constraints->permits(keyAgreementPrimitives, key))
        return false;

    return constraints->permits(
        signaturePrimitives,
        signatureAlgorithm,
        key,
        nullptr);
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

::jxx::Ptr<::jxx::ext::net::ssl::X509ExtendedTrustManager>
findExtendedTrustManager(
    const std::shared_ptr<OpenSslContextConfig>& config)
{
    if (config == nullptr || config->trustManagers == nullptr) {
        return nullptr;
    }
    for (::jxx::lang::jint index = 0;
         index < config->trustManagers->length;
         ++index)
    {
        const auto manager =
            ::jxx::CAST<
                ::jxx::ext::net::ssl::X509ExtendedTrustManager>(
                    (*config->trustManagers)[index]);
        if (manager != nullptr) {
            return manager;
        }
    }
    return nullptr;
}

::jxx::Ptr<::jxx::ext::net::ssl::X509ExtendedKeyManager>
findExtendedKeyManager(
    const std::shared_ptr<OpenSslContextConfig>& config)
{
    if (config == nullptr || config->keyManagers == nullptr) {
        return nullptr;
    }
    for (::jxx::lang::jint index = 0;
         index < config->keyManagers->length;
         ++index)
    {
        const auto manager =
            ::jxx::CAST<
                ::jxx::ext::net::ssl::X509ExtendedKeyManager>(
                    (*config->keyManagers)[index]);
        if (manager != nullptr) {
            return manager;
        }
    }
    return nullptr;
}

::jxx::Ptr<::jxx::lang::String> authenticationType(SSL* ssl) {
    const SSL_CIPHER* cipher =
        ssl == nullptr
            ? nullptr
            : SSL_get_current_cipher(ssl);
    if (cipher == nullptr) {
        return ::jxx::NEW<::jxx::lang::String>(
            "UNKNOWN");
    }
    const int nid = SSL_CIPHER_get_auth_nid(cipher);
    const char* name = OBJ_nid2sn(nid);
    return ::jxx::NEW<::jxx::lang::String>(
        name == nullptr ? "UNKNOWN" : name);
}

} // namespace

OpenSslManagerBridge::OpenSslManagerBridge(
    const std::shared_ptr<OpenSslContextConfig>& config,
    const ::jxx::Ptr<::jxx::security::AlgorithmConstraints>&
        algorithmConstraints)
    : config_(config)
    , algorithmConstraints_(algorithmConstraints) {
}

void OpenSslManagerBridge::setSocket(
    const ::jxx::Ptr<
        ::jxx::ext::net::ssl::SSLSocket>& socket)
{
    socket_ = socket;
    engine_ = nullptr;
}

void OpenSslManagerBridge::setEngine(
    const ::jxx::Ptr<
        ::jxx::ext::net::ssl::SSLEngine>& engine)
{
    engine_ = engine;
    socket_ = nullptr;
}

int OpenSslManagerBridge::verifyPeer(X509_STORE_CTX* storeContext) noexcept {
    try {
        if (X509_verify_cert(storeContext) != 1) return 0;
        const auto manager = findTrustManager(config_);
        if (manager == nullptr) return 1;

        STACK_OF(X509)* chain = X509_STORE_CTX_get0_chain(storeContext);
        const int count = chain == nullptr ? 0 : sk_X509_num(chain);
        if (count <= 0) return 0;

        if (algorithmConstraints_ != nullptr) {
            const auto signaturePrimitives = primitiveSet(
                ::jxx::security::CryptoPrimitive::SIGNATURE());
            const auto keyPrimitives = primitiveSet(
                ::jxx::security::CryptoPrimitive::PUBLIC_KEY_ENCRYPTION());
            for (int index = 0; index < count; ++index) {
                X509* certificate = sk_X509_value(chain, index);
                const int signatureNid = X509_get_signature_nid(certificate);
                const char* signatureName = OBJ_nid2sn(signatureNid);
                const auto signatureAlgorithm =
                    ::jxx::NEW<::jxx::lang::String>(
                        signatureName == nullptr ? "UNKNOWN" : signatureName);
                if (!algorithmConstraints_->permits(
                        signaturePrimitives,
                        signatureAlgorithm,
                        nullptr))
                    return 0;

                EVP_PKEY* nativeKey = X509_get_pubkey(certificate);
                if (nativeKey == nullptr) return 0;
                const auto encodedKey = encodePublicKey(nativeKey);
                const auto keyAlgorithm = publicKeyAlgorithm(nativeKey);
                EVP_PKEY_free(nativeKey);
                if (encodedKey == nullptr) return 0;
                const auto key = ::jxx::CAST<::jxx::security::Key>(
                    ::jxx::NEW<OpenSslPublicKey>(keyAlgorithm, encodedKey));
                if (!algorithmConstraints_->permits(keyPrimitives, key) ||
                    !algorithmConstraints_->permits(
                        signaturePrimitives,
                        signatureAlgorithm,
                        key,
                        nullptr))
                    return 0;
            }
        }

        const auto converted = ::jxx::NEW<
            ::jxx::ext::net::ssl::X509TrustManager::CertificateArray>(count);
        for (int index = 0; index < count; ++index) {
            const auto encoded = encodeCertificate(sk_X509_value(chain, index));
            if (encoded == nullptr) return 0;
            (*converted)[index] = ::jxx::NEW<OpenSslX509Certificate>(encoded);
        }
        SSL* ssl = static_cast<SSL*>(X509_STORE_CTX_get_ex_data(
            storeContext, SSL_get_ex_data_X509_STORE_CTX_idx()));
        const auto authType = authenticationType(ssl);
        const bool serverMode =
            ssl != nullptr && SSL_is_server(ssl);
        const auto extended =
            findExtendedTrustManager(config_);

        if (extended != nullptr && engine_ != nullptr) {
            if (serverMode) {
                extended->checkClientTrusted(
                    converted, authType, engine_);
            } else {
                extended->checkServerTrusted(
                    converted, authType, engine_);
            }
        } else if (extended != nullptr && socket_ != nullptr) {
            if (serverMode) {
                extended->checkClientTrusted(
                    converted, authType,
                    ::jxx::CAST<::jxx::net::Socket>(socket_));
            } else {
                extended->checkServerTrusted(
                    converted, authType,
                    ::jxx::CAST<::jxx::net::Socket>(socket_));
            }
        } else if (serverMode) {
            manager->checkClientTrusted(
                converted, authType);
        } else {
            manager->checkServerTrusted(
                converted, authType);
        }
        X509_STORE_CTX_set_error(storeContext, X509_V_OK);
        return 1;
    } catch (...) {
        X509_STORE_CTX_set_error(storeContext, X509_V_ERR_APPLICATION_VERIFICATION);
        return 0;
    }
}


int OpenSslManagerBridge::selectServerIdentity(SSL* ssl) noexcept {
    try {
        const auto manager = findKeyManager(config_);
        if (manager == nullptr) return 0;
        const auto keyType =
            ::jxx::NEW<::jxx::lang::String>("RSA");
        const auto extended =
            findExtendedKeyManager(config_);
        const auto alias =
            extended != nullptr && engine_ != nullptr
                ? extended->chooseEngineServerAlias(
                      keyType, nullptr, engine_)
                : manager->chooseServerAlias(
                      keyType, nullptr,
                      ::jxx::CAST<::jxx::net::Socket>(socket_));
        if (alias == nullptr) return 0;
        const auto chain = manager->getCertificateChain(alias);
        const auto key = manager->getPrivateKey(alias);
        if (chain == nullptr || chain->length == 0 || key == nullptr) return 0;
        const auto leafBytes = (*chain)[0]->getEncoded();
        const unsigned char* certCursor = reinterpret_cast<const unsigned char*>(&(*leafBytes)[0]);
        X509* leaf = d2i_X509(nullptr, &certCursor, leafBytes->length);
        const auto keyBytes = key->getEncoded();
        const unsigned char* keyCursor = reinterpret_cast<const unsigned char*>(&(*keyBytes)[0]);
        EVP_PKEY* decodedKey = d2i_AutoPrivateKey(nullptr, &keyCursor, keyBytes->length);
        if (leaf == nullptr || decodedKey == nullptr ||
            !permitsLocalIdentity(algorithmConstraints_, leaf, key) ||
            SSL_use_certificate(ssl, leaf) != 1 ||
            SSL_use_PrivateKey(ssl, decodedKey) != 1 ||
            SSL_check_private_key(ssl) != 1) {
            if (leaf != nullptr) X509_free(leaf);
            if (decodedKey != nullptr) EVP_PKEY_free(decodedKey);
            return 0;
        }
        X509_free(leaf);
        EVP_PKEY_free(decodedKey);
        for (::jxx::lang::jint index = 1; index < chain->length; ++index) {
            const auto bytes = (*chain)[index]->getEncoded();
            const unsigned char* cursor = reinterpret_cast<const unsigned char*>(&(*bytes)[0]);
            X509* extra = d2i_X509(nullptr, &cursor, bytes->length);
            if (extra == nullptr || SSL_add1_chain_cert(ssl, extra) != 1) {
                if (extra != nullptr) X509_free(extra);
                return 0;
            }
            X509_free(extra);
        }
        return 1;
    } catch (...) {
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
        const auto extended =
            findExtendedKeyManager(config_);
        const auto alias =
            extended != nullptr && engine_ != nullptr
                ? extended->chooseEngineClientAlias(
                      types, nullptr, engine_)
                : manager->chooseClientAlias(
                      types, nullptr,
                      ::jxx::CAST<::jxx::net::Socket>(socket_));
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
        if (leaf == nullptr || decodedKey == nullptr ||
            !permitsLocalIdentity(algorithmConstraints_, leaf, key)) {
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
