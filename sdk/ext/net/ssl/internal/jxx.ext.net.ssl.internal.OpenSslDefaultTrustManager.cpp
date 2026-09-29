#include <vector>
#include <openssl/x509.h>
#include <openssl/x509_vfy.h>
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslDefaultTrustManager.h"

#include "ext/net/ssl/jxx.ext.net.ssl.SSLHandshakeException.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslX509Certificate.h"
#include "security/cert/jxx.security.cert.X509Certificate.h"
#include "lang/jxx.lang.IllegalArgumentException.h"

namespace jxx::ext::net::ssl::internal {
void OpenSslDefaultTrustManager::verify(const ::jxx::Ptr<CertificateArray>& chain,
    const ::jxx::Ptr<::jxx::lang::String>& authType) {
    if (chain == nullptr || chain->length == 0 || authType == nullptr || authType->utf8().empty())
        throw ::jxx::lang::IllegalArgumentException();
    X509_STORE* store = X509_STORE_new();
    X509_STORE_CTX* context = X509_STORE_CTX_new();
    STACK_OF(X509)* untrusted = sk_X509_new_null();
    X509* leaf = nullptr;
    bool valid = store != nullptr && context != nullptr && untrusted != nullptr &&
        X509_STORE_set_default_paths(store) == 1;
    for (::jxx::lang::jint index = 0; valid && index < chain->length; ++index) {
        const auto certificate = (*chain)[index];
        const auto encoded = certificate == nullptr ? nullptr : certificate->getEncoded();
        if (encoded == nullptr || encoded->length == 0) { valid = false; break; }
        const unsigned char* cursor = reinterpret_cast<const unsigned char*>(&(*encoded)[0]);
        X509* native = d2i_X509(nullptr, &cursor, encoded->length);
        if (native == nullptr) { valid = false; break; }
        if (index == 0) leaf = native;
        else if (sk_X509_push(untrusted, native) == 0) { X509_free(native); valid = false; }
    }
    if (valid) valid = X509_STORE_CTX_init(context, store, leaf, untrusted) == 1 &&
        X509_verify_cert(context) == 1;
    if (leaf != nullptr) X509_free(leaf);
    if (untrusted != nullptr) sk_X509_pop_free(untrusted, X509_free);
    if (context != nullptr) X509_STORE_CTX_free(context);
    if (store != nullptr) X509_STORE_free(store);
    if (!valid) throw ::jxx::ext::net::ssl::SSLHandshakeException("certificate chain is not trusted");
}
void OpenSslDefaultTrustManager::checkClientTrusted(const ::jxx::Ptr<CertificateArray>& c,const ::jxx::Ptr<::jxx::lang::String>& a){verify(c,a);}
void OpenSslDefaultTrustManager::checkServerTrusted(const ::jxx::Ptr<CertificateArray>& c,const ::jxx::Ptr<::jxx::lang::String>& a){verify(c,a);}
::jxx::Ptr<OpenSslDefaultTrustManager::CertificateArray>
OpenSslDefaultTrustManager::getAcceptedIssuers() {
    X509_STORE* store = X509_STORE_new();
    if (store == nullptr || X509_STORE_set_default_paths(store) != 1) {
        if (store != nullptr) X509_STORE_free(store);
        return ::jxx::NEW<CertificateArray>(0);
    }

    STACK_OF(X509_OBJECT)* objects = X509_STORE_get0_objects(store);
    const int objectCount = objects == nullptr ? 0 : sk_X509_OBJECT_num(objects);
    std::vector<::jxx::Ptr<::jxx::security::cert::X509Certificate>> issuers;
    issuers.reserve(static_cast<std::size_t>(objectCount));

    for (int index = 0; index < objectCount; ++index) {
        X509_OBJECT* object = sk_X509_OBJECT_value(objects, index);
        X509* certificate = object == nullptr
            ? nullptr
            : X509_OBJECT_get0_X509(object);
        if (certificate == nullptr) continue;

        const int length = i2d_X509(certificate, nullptr);
        if (length <= 0) continue;
        const auto encoded = ::jxx::NEW<
            ::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>(length);
        unsigned char* cursor =
            reinterpret_cast<unsigned char*>(&(*encoded)[0]);
        if (i2d_X509(certificate, &cursor) != length) continue;
        issuers.push_back(::jxx::NEW<
            ::jxx::ext::net::ssl::internal::OpenSslX509Certificate>(encoded));
    }

    X509_STORE_free(store);
    const auto result = ::jxx::NEW<CertificateArray>(
        static_cast<::jxx::lang::jint>(issuers.size()));
    for (std::size_t index = 0; index < issuers.size(); ++index)
        (*result)[static_cast<::jxx::lang::jint>(index)] = issuers[index];
    return result;
}
} // namespace jxx::ext::net::ssl::internal
