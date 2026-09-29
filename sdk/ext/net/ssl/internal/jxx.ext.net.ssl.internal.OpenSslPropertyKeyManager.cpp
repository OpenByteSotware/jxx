#include <fstream>
#include <iterator>
#include <string>
#include <vector>
#include <openssl/evp.h>
#include <openssl/pkcs12.h>
#include <openssl/x509.h>

#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslPropertyKeyManager.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslX509Certificate.h"
#include "ext/net/ssl/jxx.ext.net.ssl.X509ExtendedKeyManager.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.System.h"
#include "security/jxx.security.PrivateKey.h"

namespace jxx::ext::net::ssl::internal {
namespace {

::jxx::lang::ByteArray copyBytes(
    const unsigned char* data,
    int length) {
    const auto result = ::jxx::NEW<
        ::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>(length);
    for (int index = 0; index < length; ++index)
        (*result)[index] = static_cast<::jxx::lang::jbyte>(data[index]);
    return result;
}

class PropertyPrivateKey final
    : public ::jxx::lang::InterfaceBase<
          PropertyPrivateKey,
          ::jxx::security::PrivateKey> {
public:
    PropertyPrivateKey(
        const ::jxx::Ptr<::jxx::lang::String>& algorithm,
        const ::jxx::lang::ByteArray& encoded)
        : algorithm_(algorithm), encoded_(encoded) {
    }

    ::jxx::Ptr<::jxx::lang::String> getAlgorithm() const override {
        return algorithm_;
    }

    ::jxx::Ptr<::jxx::lang::String> getFormat() const override {
        return ::jxx::NEW<::jxx::lang::String>("PKCS#8");
    }

    ::jxx::lang::ByteArray getEncoded() const override {
        const auto result = ::jxx::NEW<
            ::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>(encoded_->length);
        for (::jxx::lang::jint index = 0; index < encoded_->length; ++index)
            (*result)[index] = (*encoded_)[index];
        return result;
    }

private:
    ::jxx::Ptr<::jxx::lang::String> algorithm_;
    ::jxx::lang::ByteArray encoded_;
};

class PropertyKeyManager final
    : public ::jxx::lang::ClassBase<
          PropertyKeyManager,
          ::jxx::ext::net::ssl::X509ExtendedKeyManager> {
public:
    using JxxSuper = ::jxx::ext::net::ssl::X509ExtendedKeyManager;
    using Super = ::jxx::lang::ClassBase<PropertyKeyManager, JxxSuper>;
    using StringArray = JxxSuper::StringArray;
    using PrincipalArray = JxxSuper::PrincipalArray;
    using CertificateArray = JxxSuper::CertificateArray;

    PropertyKeyManager(
        const ::jxx::Ptr<CertificateArray>& chain,
        const ::jxx::Ptr<::jxx::security::PrivateKey>& key,
        const ::jxx::Ptr<::jxx::lang::String>& keyType)
        : chain_(chain), key_(key), keyType_(keyType),
          alias_(::jxx::NEW<::jxx::lang::String>("javax.net.ssl.keyStore")) {
    }

    ::jxx::Ptr<::jxx::lang::String> chooseClientAlias(
        const ::jxx::Ptr<StringArray>& keyTypes,
        const ::jxx::Ptr<PrincipalArray>&,
        const ::jxx::Ptr<::jxx::net::Socket>&) override {
        if (keyTypes == nullptr) return nullptr;
        for (::jxx::lang::jint index = 0; index < keyTypes->length; ++index)
            if ((*keyTypes)[index] != nullptr &&
                (*keyTypes)[index]->utf8() == keyType_->utf8()) return alias_;
        return nullptr;
    }

    ::jxx::Ptr<::jxx::lang::String> chooseServerAlias(
        const ::jxx::Ptr<::jxx::lang::String>& keyType,
        const ::jxx::Ptr<PrincipalArray>&,
        const ::jxx::Ptr<::jxx::net::Socket>&) override {
        return keyType != nullptr && keyType->utf8() == keyType_->utf8()
            ? alias_ : nullptr;
    }

    ::jxx::Ptr<CertificateArray> getCertificateChain(
        const ::jxx::Ptr<::jxx::lang::String>& alias) override {
        if (alias == nullptr || alias->utf8() != alias_->utf8()) return nullptr;
        const auto copy = ::jxx::NEW<CertificateArray>(chain_->length);
        for (::jxx::lang::jint index = 0; index < chain_->length; ++index)
            (*copy)[index] = (*chain_)[index];
        return copy;
    }

    ::jxx::Ptr<StringArray> getClientAliases(
        const ::jxx::Ptr<::jxx::lang::String>& keyType,
        const ::jxx::Ptr<PrincipalArray>&) override {
        if (keyType == nullptr || keyType->utf8() != keyType_->utf8()) return nullptr;
        const auto result = ::jxx::NEW<StringArray>(1);
        (*result)[0] = alias_;
        return result;
    }

    ::jxx::Ptr<::jxx::security::PrivateKey> getPrivateKey(
        const ::jxx::Ptr<::jxx::lang::String>& alias) override {
        return alias != nullptr && alias->utf8() == alias_->utf8()
            ? key_ : nullptr;
    }

    ::jxx::Ptr<StringArray> getServerAliases(
        const ::jxx::Ptr<::jxx::lang::String>& keyType,
        const ::jxx::Ptr<PrincipalArray>& issuers) override {
        return getClientAliases(keyType, issuers);
    }

    ::jxx::Ptr<::jxx::lang::String> chooseEngineClientAlias(
        const ::jxx::Ptr<StringArray>& keyTypes,
        const ::jxx::Ptr<PrincipalArray>& issuers,
        const ::jxx::Ptr<::jxx::ext::net::ssl::SSLEngine>&) override {
        return chooseClientAlias(keyTypes, issuers, nullptr);
    }

    ::jxx::Ptr<::jxx::lang::String> chooseEngineServerAlias(
        const ::jxx::Ptr<::jxx::lang::String>& keyType,
        const ::jxx::Ptr<PrincipalArray>& issuers,
        const ::jxx::Ptr<::jxx::ext::net::ssl::SSLEngine>&) override {
        return chooseServerAlias(keyType, issuers, nullptr);
    }

private:
    ::jxx::Ptr<CertificateArray> chain_;
    ::jxx::Ptr<::jxx::security::PrivateKey> key_;
    ::jxx::Ptr<::jxx::lang::String> keyType_;
    ::jxx::Ptr<::jxx::lang::String> alias_;
};

} // namespace

::jxx::Ptr<::jxx::ext::net::ssl::SSLContext::KeyManagerArray>
loadDefaultPropertyKeyManagers() {
    const auto path = ::jxx::lang::System::getProperty(
        ::jxx::NEW<::jxx::lang::String>("javax.net.ssl.keyStore"));
    if (path == nullptr || path->utf8().empty()) return nullptr;

    const auto passwordValue = ::jxx::lang::System::getProperty(
        ::jxx::NEW<::jxx::lang::String>("javax.net.ssl.keyStorePassword"));
    const std::string password = passwordValue == nullptr
        ? std::string() : passwordValue->utf8();

    std::ifstream stream(path->utf8(), std::ios::binary);
    if (!stream)
        throw ::jxx::lang::IllegalStateException(
            "cannot open javax.net.ssl.keyStore");
    const std::vector<unsigned char> bytes{
        std::istreambuf_iterator<char>(stream),
        std::istreambuf_iterator<char>()};
    if (bytes.empty())
        throw ::jxx::lang::IllegalStateException(
            "javax.net.ssl.keyStore is empty");

    const unsigned char* cursor = bytes.data();
    PKCS12* container = d2i_PKCS12(nullptr, &cursor,
        static_cast<long>(bytes.size()));
    if (container == nullptr || cursor != bytes.data() + bytes.size()) {
        if (container != nullptr) PKCS12_free(container);
        throw ::jxx::lang::IllegalStateException(
            "invalid PKCS12 javax.net.ssl.keyStore");
    }

    EVP_PKEY* nativeKey = nullptr;
    X509* leaf = nullptr;
    STACK_OF(X509)* extras = nullptr;
    const int parsed = PKCS12_parse(container, password.c_str(),
        &nativeKey, &leaf, &extras);
    PKCS12_free(container);
    if (parsed != 1 || nativeKey == nullptr || leaf == nullptr) {
        if (nativeKey != nullptr) EVP_PKEY_free(nativeKey);
        if (leaf != nullptr) X509_free(leaf);
        if (extras != nullptr) sk_X509_pop_free(extras, X509_free);
        throw ::jxx::lang::IllegalStateException(
            "cannot unlock javax.net.ssl.keyStore");
    }

    const int keyLength = i2d_PrivateKey(nativeKey, nullptr);
    const int leafLength = i2d_X509(leaf, nullptr);
    if (keyLength <= 0 || leafLength <= 0) {
        EVP_PKEY_free(nativeKey);
        X509_free(leaf);
        if (extras != nullptr) sk_X509_pop_free(extras, X509_free);
        throw ::jxx::lang::IllegalStateException(
            "cannot encode PKCS12 identity");
    }

    std::vector<unsigned char> keyBytes(static_cast<std::size_t>(keyLength));
    unsigned char* keyOutput = keyBytes.data();
    i2d_PrivateKey(nativeKey, &keyOutput);
    const int keyId = EVP_PKEY_base_id(nativeKey);
    const char* algorithmName =
        keyId == EVP_PKEY_RSA
            ? "RSA"
            : keyId == EVP_PKEY_EC
                ? "EC"
                : OBJ_nid2sn(keyId);
    const auto key = ::jxx::NEW<PropertyPrivateKey>(
        ::jxx::NEW<::jxx::lang::String>(
            algorithmName == nullptr ? "UNKNOWN" : algorithmName),
        copyBytes(keyBytes.data(), keyLength));

    const int extraCount = extras == nullptr ? 0 : sk_X509_num(extras);
    const auto chain = ::jxx::NEW<PropertyKeyManager::CertificateArray>(
        static_cast<::jxx::lang::jint>(1 + extraCount));
    std::vector<unsigned char> leafBytes(static_cast<std::size_t>(leafLength));
    unsigned char* leafOutput = leafBytes.data();
    i2d_X509(leaf, &leafOutput);
    (*chain)[0] = ::jxx::NEW<OpenSslX509Certificate>(
        copyBytes(leafBytes.data(), leafLength));
    for (int index = 0; index < extraCount; ++index) {
        X509* certificate = sk_X509_value(extras, index);
        const int length = i2d_X509(certificate, nullptr);
        if (length <= 0) continue;
        std::vector<unsigned char> encoded(static_cast<std::size_t>(length));
        unsigned char* output = encoded.data();
        i2d_X509(certificate, &output);
        (*chain)[static_cast<::jxx::lang::jint>(index + 1)] =
            ::jxx::NEW<OpenSslX509Certificate>(
                copyBytes(encoded.data(), length));
    }

    EVP_PKEY_free(nativeKey);
    X509_free(leaf);
    if (extras != nullptr) sk_X509_pop_free(extras, X509_free);

    const auto managers = ::jxx::NEW<
        ::jxx::ext::net::ssl::SSLContext::KeyManagerArray>(1);
    (*managers)[0] = ::jxx::NEW<PropertyKeyManager>(
        chain, key,
        ::jxx::NEW<::jxx::lang::String>(
            algorithmName == nullptr ? "UNKNOWN" : algorithmName));
    return managers;
}

} // namespace jxx::ext::net::ssl::internal
