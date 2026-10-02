#include <algorithm>
#include <cctype>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

#include <openssl/pem.h>
#include <openssl/pkcs12.h>
#include <openssl/x509.h>
#include <openssl/x509_vfy.h>
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslPropertyTrustManager.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslX509Certificate.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLHandshakeException.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.System.h"
#include "security/cert/jxx.security.cert.X509Certificate.h"
#include "security/jxx.security.KeyStore.h"

namespace jxx::ext::net::ssl::internal {
namespace {

::jxx::lang::ByteArray encodeCertificate(X509* certificate) {
    if (certificate == nullptr) return nullptr;
    const int length = i2d_X509(certificate, nullptr);
    if (length <= 0) return nullptr;
    const auto result = ::jxx::NEW<
        ::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>(length);
    unsigned char* cursor =
        reinterpret_cast<unsigned char*>(&(*result)[0]);
    return i2d_X509(certificate, &cursor) == length
        ? result : nullptr;
}

X509* decodeCertificate(const ::jxx::lang::ByteArray& encoded) {
    if (encoded == nullptr || encoded->length == 0) return nullptr;
    const unsigned char* cursor =
        reinterpret_cast<const unsigned char*>(&(*encoded)[0]);
    return d2i_X509(nullptr, &cursor, encoded->length);
}

std::vector<::jxx::lang::ByteArray> readTrustCertificates(
    const std::string& path,
    const std::string& password,
    const std::string& type) {
    std::ifstream stream(path, std::ios::binary);
    if (!stream)
        throw ::jxx::lang::IllegalStateException(
            "cannot open javax.net.ssl.trustStore");
    const std::vector<unsigned char> bytes{
        std::istreambuf_iterator<char>(stream),
        std::istreambuf_iterator<char>()};
    if (bytes.empty())
        throw ::jxx::lang::IllegalStateException(
            "javax.net.ssl.trustStore is empty");

    std::vector<::jxx::lang::ByteArray> certificates;

    const bool pkcs12 = type == "PKCS12" || type == "PKCS#12";
    const bool pem = type == "PEM" || type == "X509" || type == "X.509";
    const bool der = type == "DER" || type == "X509" || type == "X.509";

    const unsigned char* cursor = bytes.data();
    PKCS12* container = pkcs12
        ? d2i_PKCS12(nullptr, &cursor, static_cast<long>(bytes.size()))
        : nullptr;
    if (container != nullptr && cursor == bytes.data() + bytes.size()) {
        EVP_PKEY* key = nullptr;
        X509* leaf = nullptr;
        STACK_OF(X509)* extras = nullptr;
        if (PKCS12_parse(container, password.c_str(), &key, &leaf, &extras) == 1) {
            const auto leafBytes = encodeCertificate(leaf);
            if (leafBytes != nullptr) certificates.push_back(leafBytes);
            const int count = extras == nullptr ? 0 : sk_X509_num(extras);
            for (int index = 0; index < count; ++index) {
                const auto encoded = encodeCertificate(
                    sk_X509_value(extras, index));
                if (encoded != nullptr) certificates.push_back(encoded);
            }
        }
        if (key != nullptr) EVP_PKEY_free(key);
        if (leaf != nullptr) X509_free(leaf);
        if (extras != nullptr) sk_X509_pop_free(extras, X509_free);
        PKCS12_free(container);
    } else if (container != nullptr) {
        PKCS12_free(container);
    }

    if (certificates.empty() && pem) {
        BIO* memory = BIO_new_mem_buf(
            bytes.data(), static_cast<int>(bytes.size()));
        if (memory != nullptr) {
            for (;;) {
                X509* certificate = PEM_read_bio_X509(
                    memory, nullptr, nullptr, nullptr);
                if (certificate == nullptr) break;
                const auto encoded = encodeCertificate(certificate);
                if (encoded != nullptr) certificates.push_back(encoded);
                X509_free(certificate);
            }
            BIO_free(memory);
        }
    }

    if (certificates.empty() && der) {
        cursor = bytes.data();
        X509* certificate = d2i_X509(
            nullptr, &cursor, static_cast<long>(bytes.size()));
        if (certificate != nullptr && cursor == bytes.data() + bytes.size()) {
            const auto encoded = encodeCertificate(certificate);
            if (encoded != nullptr) certificates.push_back(encoded);
        }
        if (certificate != nullptr) X509_free(certificate);
    }

    if (certificates.empty())
        throw ::jxx::lang::IllegalStateException(
            "javax.net.ssl.trustStore contains no X.509 certificates");
    return certificates;
}

} // namespace

OpenSslPropertyTrustManager::OpenSslPropertyTrustManager(
    const std::vector<::jxx::lang::ByteArray>& trustedCertificates)
    : trustedCertificates_(trustedCertificates) {
}

void OpenSslPropertyTrustManager::checkClientTrusted(
    const ::jxx::Ptr<CertificateArray>& chain,
    const ::jxx::Ptr<::jxx::lang::String>& authType) {
    verify(chain, authType);
}

void OpenSslPropertyTrustManager::checkServerTrusted(
    const ::jxx::Ptr<CertificateArray>& chain,
    const ::jxx::Ptr<::jxx::lang::String>& authType) {
    verify(chain, authType);
}

void OpenSslPropertyTrustManager::verify(
    const ::jxx::Ptr<CertificateArray>& chain,
    const ::jxx::Ptr<::jxx::lang::String>& authType) const {
    if (chain == nullptr || chain->length == 0 ||
        authType == nullptr || authType->utf8().empty())
        throw ::jxx::lang::IllegalArgumentException();

    X509_STORE* store = X509_STORE_new();
    X509_STORE_CTX* context = X509_STORE_CTX_new();
    STACK_OF(X509)* untrusted = sk_X509_new_null();
    X509* leaf = nullptr;
    bool ok = store != nullptr && context != nullptr && untrusted != nullptr;

    for (const auto& trusted : trustedCertificates_) {
        X509* certificate = decodeCertificate(trusted);
        if (certificate == nullptr || X509_STORE_add_cert(store, certificate) != 1)
            ok = false;
        if (certificate != nullptr) X509_free(certificate);
        if (!ok) break;
    }

    if (ok) leaf = decodeCertificate((*chain)[0]->getEncoded());
    ok = ok && leaf != nullptr;
    for (::jxx::lang::jint index = 1; ok && index < chain->length; ++index) {
        X509* certificate = (*chain)[index] == nullptr
            ? nullptr : decodeCertificate((*chain)[index]->getEncoded());
        if (certificate == nullptr || sk_X509_push(untrusted, certificate) != 1) {
            if (certificate != nullptr) X509_free(certificate);
            ok = false;
        }
    }

    if (ok) ok = X509_STORE_CTX_init(context, store, leaf, untrusted) == 1;
    if (ok) ok = X509_verify_cert(context) == 1;

    if (leaf != nullptr) X509_free(leaf);
    if (untrusted != nullptr) sk_X509_pop_free(untrusted, X509_free);
    if (context != nullptr) X509_STORE_CTX_free(context);
    if (store != nullptr) X509_STORE_free(store);

    if (!ok)
        throw ::jxx::ext::net::ssl::SSLHandshakeException(
            "certificate chain is not trusted by javax.net.ssl.trustStore");
}

::jxx::Ptr<OpenSslPropertyTrustManager::CertificateArray>
OpenSslPropertyTrustManager::getAcceptedIssuers() {
    const auto result = ::jxx::NEW<CertificateArray>(
        static_cast<::jxx::lang::jint>(trustedCertificates_.size()));
    for (std::size_t index = 0; index < trustedCertificates_.size(); ++index)
        (*result)[static_cast<::jxx::lang::jint>(index)] =
            ::jxx::NEW<OpenSslX509Certificate>(trustedCertificates_[index]);
    return result;
}

std::string discoverDefaultTrustStorePath(
    const std::string& runtimeHome) {
    if (runtimeHome.empty()) return std::string();
#ifdef _WIN32
    const char separator = '\\';
#else
    const char separator = '/';
#endif
    const std::vector<std::string> candidates{
        runtimeHome + separator + "lib" + separator + "security" + separator + "jssecacerts",
        runtimeHome + separator + "lib" + separator + "security" + separator + "cacerts",
        runtimeHome + separator + "jre" + separator + "lib" + separator + "security" + separator + "jssecacerts",
        runtimeHome + separator + "jre" + separator + "lib" + separator + "security" + separator + "cacerts"};
    for (const auto& candidate : candidates) {
        std::ifstream stream(candidate, std::ios::binary);
        if (stream.good()) return candidate;
    }
    return std::string();
}

::jxx::Ptr<::jxx::ext::net::ssl::SSLContext::TrustManagerArray>
loadDefaultPropertyTrustManagers() {
    const auto configuredPath = ::jxx::lang::System::getProperty(
        ::jxx::NEW<::jxx::lang::String>("javax.net.ssl.trustStore"));
    const bool explicitlyConfigured = configuredPath != nullptr;
    std::string path = explicitlyConfigured
        ? configuredPath->utf8()
        : std::string();

    const auto emptyManagers = []() {
        const auto managers = ::jxx::NEW<
            ::jxx::ext::net::ssl::SSLContext::TrustManagerArray>(1);
        (*managers)[0] = ::jxx::NEW<OpenSslPropertyTrustManager>(
            std::vector<::jxx::lang::ByteArray>());
        return managers;
    };

    if (explicitlyConfigured) {
        if (path == "NONE" || path.empty()) return emptyManagers();
        std::ifstream configured(path, std::ios::binary);
        if (!configured.good()) return emptyManagers();
    } else {
        const auto homeValue = ::jxx::lang::System::getProperty(
            ::jxx::NEW<::jxx::lang::String>("java.home"));
        if (homeValue != nullptr)
            path = discoverDefaultTrustStorePath(homeValue->utf8());
        if (path.empty()) return nullptr;
    }

    const auto typeValue = ::jxx::lang::System::getProperty(
        ::jxx::NEW<::jxx::lang::String>("javax.net.ssl.trustStoreType"));
    std::string type = typeValue == nullptr || typeValue->utf8().empty()
        ? ::jxx::security::KeyStore::getDefaultType()->utf8()
        : typeValue->utf8();
    std::transform(type.begin(), type.end(), type.begin(),
        [](unsigned char value) { return static_cast<char>(std::toupper(value)); });
    if (type != "PKCS12" && type != "PKCS#12" &&
        type != "PEM" && type != "X509" && type != "X.509" &&
        type != "DER")
        throw ::jxx::lang::IllegalStateException(
            "unsupported javax.net.ssl.trustStoreType");

    const auto providerValue = ::jxx::lang::System::getProperty(
        ::jxx::NEW<::jxx::lang::String>("javax.net.ssl.trustStoreProvider"));
    if (providerValue != nullptr && !providerValue->utf8().empty()) {
        std::string provider = providerValue->utf8();
        std::transform(provider.begin(), provider.end(), provider.begin(),
            [](unsigned char value) { return static_cast<char>(std::toupper(value)); });
        if (provider != "JXX" && provider != "OPENSSL" &&
            provider != "SUN" && provider != "SUNJSSE")
            throw ::jxx::lang::IllegalStateException(
                "unsupported javax.net.ssl.trustStoreProvider");
    }

    const auto passwordValue = ::jxx::lang::System::getProperty(
        ::jxx::NEW<::jxx::lang::String>("javax.net.ssl.trustStorePassword"));
    const auto certificates = readTrustCertificates(
        path,
        passwordValue == nullptr ? std::string() : passwordValue->utf8(),
        type);
    const auto managers = ::jxx::NEW<
        ::jxx::ext::net::ssl::SSLContext::TrustManagerArray>(1);
    (*managers)[0] = ::jxx::NEW<OpenSslPropertyTrustManager>(certificates);
    return managers;
}

} // namespace jxx::ext::net::ssl::internal
