#include "security/cert/jxx.security.cert.Certificate.h"

#include <openssl/evp.h>
#include <openssl/x509.h>

#include <sstream>

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "security/jxx.security.GeneralSecurityException.h"
#include "security/jxx.security.Provider.h"
#include "security/jxx.security.PublicKey.h"

namespace jxx::security::cert {
namespace {

void verifyEncodedCertificate(
    const ::jxx::lang::ByteArray& certificateBytes,
    const ::jxx::Ptr<::jxx::security::PublicKey>& key) {
    if (key == nullptr) throw ::jxx::lang::NullPointerException();
    const auto keyBytes = key->getEncoded();
    if (certificateBytes == nullptr || certificateBytes->length <= 0 ||
        keyBytes == nullptr || keyBytes->length <= 0)
        throw ::jxx::security::GeneralSecurityException(
            "certificate or public-key encoding is unavailable");

    const unsigned char* certificateCursor =
        reinterpret_cast<const unsigned char*>(&(*certificateBytes)[0]);
    X509* certificate = d2i_X509(
        nullptr, &certificateCursor, certificateBytes->length);
    const unsigned char* keyCursor =
        reinterpret_cast<const unsigned char*>(&(*keyBytes)[0]);
    EVP_PKEY* nativeKey = d2i_PUBKEY(
        nullptr, &keyCursor, keyBytes->length);
    if (certificate == nullptr || nativeKey == nullptr) {
        if (certificate != nullptr) X509_free(certificate);
        if (nativeKey != nullptr) EVP_PKEY_free(nativeKey);
        throw ::jxx::security::GeneralSecurityException(
            "invalid certificate or public-key encoding");
    }
    const int verified = X509_verify(certificate, nativeKey);
    EVP_PKEY_free(nativeKey);
    X509_free(certificate);
    if (verified != 1)
        throw ::jxx::security::GeneralSecurityException(
            "certificate signature verification failed");
}

} // namespace

Certificate::Certificate(
    const ::jxx::Ptr<::jxx::lang::String>& type)
    : type_(type) {
    if (type_ == nullptr) throw ::jxx::lang::NullPointerException();
}

::jxx::Ptr<::jxx::lang::String> Certificate::getType() const {
    return type_;
}

void Certificate::verify(
    const ::jxx::Ptr<::jxx::security::PublicKey>& key) const {
    verifyEncodedCertificate(getEncoded(), key);
}

void Certificate::verify(
    const ::jxx::Ptr<::jxx::security::PublicKey>& key,
    const ::jxx::Ptr<::jxx::lang::String>& provider) const {
    if (provider == nullptr) throw ::jxx::lang::NullPointerException();
    if (provider->utf8().empty())
        throw ::jxx::lang::IllegalArgumentException(
            "provider name is empty");
    verifyEncodedCertificate(getEncoded(), key);
}

void Certificate::verify(
    const ::jxx::Ptr<::jxx::security::PublicKey>& key,
    const ::jxx::Ptr<::jxx::security::Provider>& provider) const {
    if (provider == nullptr) throw ::jxx::lang::NullPointerException();
    verifyEncodedCertificate(getEncoded(), key);
}

::jxx::lang::jbool Certificate::equals(
    const ::jxx::Ptr<::jxx::lang::Object>& other) const {
    if (other.get() == this) return true;
    const auto certificate = ::jxx::CAST<Certificate>(other);
    if (certificate == nullptr) return false;
    const auto left = getEncoded();
    const auto right = certificate->getEncoded();
    if (left == nullptr || right == nullptr || left->length != right->length)
        return false;
    for (::jxx::lang::jint index = 0; index < left->length; ++index)
        if ((*left)[index] != (*right)[index]) return false;
    return true;
}

::jxx::lang::jint Certificate::hashCode() const {
    const auto encoded = getEncoded();
    std::uint32_t hash = 1U;
    if (encoded != nullptr)
        for (::jxx::lang::jint index = 0; index < encoded->length; ++index)
            hash = hash * 31U +
                static_cast<std::uint8_t>((*encoded)[index]);
    return static_cast<::jxx::lang::jint>(hash);
}

::jxx::Ptr<::jxx::lang::String> Certificate::toString() const {
    const auto encoded = getEncoded();
    std::ostringstream stream;
    stream << "Certificate[type=" << type_->utf8()
           << ", encodedLength="
           << (encoded == nullptr ? 0 : encoded->length) << "]";
    return ::jxx::NEW<::jxx::lang::String>(stream.str());
}

} // namespace jxx::security::cert
