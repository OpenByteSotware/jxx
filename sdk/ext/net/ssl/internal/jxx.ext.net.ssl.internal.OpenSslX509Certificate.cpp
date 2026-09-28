#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslX509Certificate.h"

#include <openssl/asn1.h>
#include <openssl/bn.h>
#include <openssl/evp.h>
#include <openssl/objects.h>
#include <openssl/x509.h>

#include <ctime>
#include <string>

#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslPublicKey.h"
#include "math/jxx.math.BigInteger.h"
#include "util/jxx.util.Date.h"
#include "io/jxx.io.IOException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.UnsupportedOperationException.h"

namespace jxx::ext::net::ssl::internal {

OpenSslX509Certificate::OpenSslX509Certificate(
    const ::jxx::lang::ByteArray& encoded)
    : Super(::jxx::NEW<::jxx::lang::String>("X.509"))
    , encoded_(encoded) {
    if (encoded_ == nullptr) throw ::jxx::lang::NullPointerException();
}

::jxx::lang::ByteArray OpenSslX509Certificate::getEncoded() const {
    const auto copy = ::jxx::NEW<
        ::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>(encoded_->length);
    for (::jxx::lang::jint index = 0; index < encoded_->length; ++index)
        (*copy)[index] = (*encoded_)[index];
    return copy;
}

void OpenSslX509Certificate::checkValidity() const {
    const unsigned char* cursor = reinterpret_cast<const unsigned char*>(&(*encoded_)[0]);
    X509* certificate = d2i_X509(nullptr, &cursor, encoded_->length);
    if (certificate == nullptr) throw ::jxx::io::IOException("Invalid DER X.509 certificate");
    const bool valid = X509_cmp_current_time(X509_get0_notBefore(certificate)) <= 0 &&
        X509_cmp_current_time(X509_get0_notAfter(certificate)) >= 0;
    X509_free(certificate);
    if (!valid) throw ::jxx::io::IOException("X.509 certificate is not currently valid");
}

namespace {

X509* decodeCertificate(const ::jxx::lang::ByteArray& encoded) {
    const unsigned char* cursor =
        reinterpret_cast<const unsigned char*>(&(*encoded)[0]);
    X509* certificate = d2i_X509(nullptr, &cursor, encoded->length);
    if (certificate == nullptr)
        throw ::jxx::io::IOException("Invalid DER X.509 certificate");
    return certificate;
}

::jxx::lang::jlong epochMillis(const ASN1_TIME* value) {
    std::tm time{};
    if (value == nullptr || ASN1_TIME_to_tm(value, &time) != 1)
        throw ::jxx::io::IOException("Invalid X.509 validity time");
#ifdef _WIN32
    const std::time_t epoch = _mkgmtime(&time);
#else
    const std::time_t epoch = timegm(&time);
#endif
    if (epoch == static_cast<std::time_t>(-1))
        throw ::jxx::io::IOException("X.509 validity time is out of range");
    return static_cast<::jxx::lang::jlong>(epoch) * 1000;
}

::jxx::lang::ByteArray copyBytes(
    const unsigned char* data,
    int length) {
    if (data == nullptr || length < 0) return nullptr;
    const auto result = ::jxx::NEW<
        ::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>(length);
    for (int index = 0; index < length; ++index)
        (*result)[index] = static_cast<::jxx::lang::jbyte>(data[index]);
    return result;
}

} // namespace

::jxx::lang::jint OpenSslX509Certificate::getVersion() const {
    X509* certificate = decodeCertificate(encoded_);
    const auto version = static_cast<::jxx::lang::jint>(
        X509_get_version(certificate) + 1);
    X509_free(certificate);
    return version;
}

::jxx::Ptr<::jxx::math::BigInteger>
OpenSslX509Certificate::getSerialNumber() const {
    X509* certificate = decodeCertificate(encoded_);
    BIGNUM* number = ASN1_INTEGER_to_BN(
        X509_get_serialNumber(certificate), nullptr);
    X509_free(certificate);
    if (number == nullptr)
        throw ::jxx::io::IOException("Unable to decode X.509 serial number");
    char* decimal = BN_bn2dec(number);
    BN_free(number);
    if (decimal == nullptr)
        throw ::jxx::io::IOException("Unable to format X.509 serial number");
    const auto result = ::jxx::NEW<::jxx::math::BigInteger>(
        ::jxx::NEW<::jxx::lang::String>(decimal));
    OPENSSL_free(decimal);
    return result;
}

::jxx::Ptr<::jxx::util::Date>
OpenSslX509Certificate::getNotBefore() const {
    X509* certificate = decodeCertificate(encoded_);
    const auto millis = epochMillis(X509_get0_notBefore(certificate));
    X509_free(certificate);
    return ::jxx::NEW<::jxx::util::Date>(millis);
}

::jxx::Ptr<::jxx::util::Date>
OpenSslX509Certificate::getNotAfter() const {
    X509* certificate = decodeCertificate(encoded_);
    const auto millis = epochMillis(X509_get0_notAfter(certificate));
    X509_free(certificate);
    return ::jxx::NEW<::jxx::util::Date>(millis);
}

::jxx::lang::ByteArray OpenSslX509Certificate::getSignature() const {
    X509* certificate = decodeCertificate(encoded_);
    const ASN1_BIT_STRING* signature = nullptr;
    const X509_ALGOR* algorithm = nullptr;
    X509_get0_signature(&signature, &algorithm, certificate);
    const auto result = signature == nullptr
        ? nullptr
        : copyBytes(signature->data, signature->length);
    X509_free(certificate);
    return result;
}

::jxx::Ptr<::jxx::lang::String>
OpenSslX509Certificate::getSigAlgName() const {
    X509* certificate = decodeCertificate(encoded_);
    const int nid = X509_get_signature_nid(certificate);
    const char* name = OBJ_nid2ln(nid);
    if (name == nullptr) name = OBJ_nid2sn(nid);
    const auto result = ::jxx::NEW<::jxx::lang::String>(
        name == nullptr ? "UNKNOWN" : name);
    X509_free(certificate);
    return result;
}

::jxx::Ptr<::jxx::lang::String>
OpenSslX509Certificate::getSigAlgOID() const {
    X509* certificate = decodeCertificate(encoded_);
    const ASN1_BIT_STRING* signature = nullptr;
    const X509_ALGOR* algorithm = nullptr;
    X509_get0_signature(&signature, &algorithm, certificate);
    const ASN1_OBJECT* object = nullptr;
    X509_ALGOR_get0(&object, nullptr, nullptr, algorithm);
    char buffer[128]{};
    const int length = object == nullptr
        ? 0
        : OBJ_obj2txt(buffer, sizeof(buffer), object, 1);
    X509_free(certificate);
    if (length <= 0)
        throw ::jxx::io::IOException("Unable to read X.509 signature OID");
    return ::jxx::NEW<::jxx::lang::String>(buffer);
}

::jxx::lang::ByteArray OpenSslX509Certificate::getSigAlgParams() const {
    X509* certificate = decodeCertificate(encoded_);
    const ASN1_BIT_STRING* signature = nullptr;
    const X509_ALGOR* algorithm = nullptr;
    X509_get0_signature(&signature, &algorithm, certificate);
    const void* parameterValue = nullptr;
    int parameterType = V_ASN1_UNDEF;
    X509_ALGOR_get0(nullptr, &parameterType, &parameterValue, algorithm);
    if (parameterType == V_ASN1_UNDEF || parameterType == V_ASN1_NULL) {
        X509_free(certificate);
        return nullptr;
    }
    ASN1_TYPE parameter{};
    parameter.type = parameterType;
    parameter.value.ptr = const_cast<char*>(
        static_cast<const char*>(parameterValue));
    const int length = i2d_ASN1_TYPE(&parameter, nullptr);
    if (length <= 0) {
        X509_free(certificate);
        return nullptr;
    }
    const auto result = ::jxx::NEW<
        ::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>(length);
    unsigned char* cursor =
        reinterpret_cast<unsigned char*>(&(*result)[0]);
    i2d_ASN1_TYPE(&parameter, &cursor);
    X509_free(certificate);
    return result;
}

::jxx::Ptr<::jxx::security::PublicKey>
OpenSslX509Certificate::getPublicKey() const {
    X509* certificate = decodeCertificate(encoded_);
    EVP_PKEY* key = X509_get_pubkey(certificate);
    X509_free(certificate);
    if (key == nullptr)
        throw ::jxx::io::IOException("X.509 certificate has no public key");
    const int length = i2d_PUBKEY(key, nullptr);
    const int nid = EVP_PKEY_base_id(key);
    const char* algorithm = OBJ_nid2sn(nid);
    if (length <= 0) {
        EVP_PKEY_free(key);
        throw ::jxx::io::IOException("Unable to encode X.509 public key");
    }
    const auto encoded = ::jxx::NEW<
        ::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>(length);
    unsigned char* cursor =
        reinterpret_cast<unsigned char*>(&(*encoded)[0]);
    if (i2d_PUBKEY(key, &cursor) != length) {
        EVP_PKEY_free(key);
        throw ::jxx::io::IOException("Unable to encode X.509 public key");
    }
    EVP_PKEY_free(key);
    return ::jxx::NEW<OpenSslPublicKey>(
        ::jxx::NEW<::jxx::lang::String>(
            algorithm == nullptr ? "UNKNOWN" : algorithm),
        encoded);
}

::jxx::Ptr<::jxx::security::Principal>
OpenSslX509Certificate::getIssuerDN() const { return nullptr; }

::jxx::Ptr<::jxx::security::Principal>
OpenSslX509Certificate::getSubjectDN() const { return nullptr; }

void OpenSslX509Certificate::writeObject(
    const ::jxx::Ptr<::jxx::io::ObjectOutputStream>&) {
    throw ::jxx::lang::UnsupportedOperationException();
}
void OpenSslX509Certificate::readObject(
    const ::jxx::Ptr<::jxx::io::ObjectInputStream>&) {
    throw ::jxx::lang::UnsupportedOperationException();
}
void OpenSslX509Certificate::readObjectNoData() {
    throw ::jxx::lang::UnsupportedOperationException();
}

} // namespace jxx::ext::net::ssl::internal
