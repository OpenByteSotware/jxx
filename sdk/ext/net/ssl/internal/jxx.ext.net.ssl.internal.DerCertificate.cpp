#include <openssl/x509.h>


#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslPublicKey.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.DerCertificate.h"

namespace jxx::ext::net::ssl::internal {
namespace {

::jxx::lang::ByteArray copyBytes(
    const ::jxx::lang::ByteArray& source) {
    if (source == nullptr)
        throw ::jxx::lang::NullPointerException();
    const auto copy = ::jxx::NEW<
        ::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>(
            source->length);
    for (::jxx::lang::jint index = 0; index < source->length; ++index)
        (*copy)[index] = (*source)[index];
    return copy;
}

void validateX509Der(
    const ::jxx::lang::ByteArray& encoded) {
    if (encoded == nullptr)
        throw ::jxx::lang::NullPointerException();
    if (encoded->length <= 0)
        throw ::jxx::lang::IllegalArgumentException(
            "X.509 DER encoding is empty");
    const unsigned char* begin =
        reinterpret_cast<const unsigned char*>(&(*encoded)[0]);
    const unsigned char* cursor = begin;
    X509* certificate = d2i_X509(nullptr, &cursor, encoded->length);
    const bool complete = certificate != nullptr &&
        cursor == begin + encoded->length;
    if (certificate != nullptr) X509_free(certificate);
    if (!complete)
        throw ::jxx::lang::IllegalArgumentException(
            "invalid X.509 DER encoding");
}

} // namespace

DerCertificate::DerCertificate(
    const ::jxx::lang::ByteArray& encoded)
    : Super(::jxx::NEW<::jxx::lang::String>("X.509")) {
    validateX509Der(encoded);
    encoded_ = copyBytes(encoded);
}

::jxx::lang::ByteArray DerCertificate::getEncoded() const {
    return copyBytes(encoded_);
}

::jxx::Ptr<::jxx::security::PublicKey>
DerCertificate::getPublicKey() const {
    const unsigned char* cursor =
        reinterpret_cast<const unsigned char*>(&(*encoded_)[0]);
    X509* certificate = d2i_X509(nullptr, &cursor, encoded_->length);
    EVP_PKEY* key = certificate == nullptr
        ? nullptr : X509_get_pubkey(certificate);
    if (certificate != nullptr) X509_free(certificate);
    if (key == nullptr)
        throw ::jxx::lang::IllegalStateException(
            "X.509 certificate has no public key");
    const int length = i2d_PUBKEY(key, nullptr);
    const char* algorithm = OBJ_nid2sn(EVP_PKEY_base_id(key));
    if (length <= 0) {
        EVP_PKEY_free(key);
        throw ::jxx::lang::IllegalStateException(
            "unable to encode X.509 public key");
    }
    const auto encoded = ::jxx::NEW<
        ::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>(length);
    unsigned char* output =
        reinterpret_cast<unsigned char*>(&(*encoded)[0]);
    if (i2d_PUBKEY(key, &output) != length) {
        EVP_PKEY_free(key);
        throw ::jxx::lang::IllegalStateException(
            "unable to encode X.509 public key");
    }
    EVP_PKEY_free(key);
    return ::jxx::NEW<OpenSslPublicKey>(
        ::jxx::NEW<::jxx::lang::String>(
            algorithm == nullptr ? "UNKNOWN" : algorithm),
        encoded);
}

void DerCertificate::writeObject(
    const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& output) {
    if (output == nullptr)
        throw ::jxx::lang::NullPointerException();
    output->writeInt(encoded_->length);
    output->write(encoded_);
}

void DerCertificate::readObject(
    const ::jxx::Ptr<::jxx::io::ObjectInputStream>& input) {
    if (input == nullptr)
        throw ::jxx::lang::NullPointerException();
    const auto length = input->readInt();
    if (length <= 0)
        throw ::jxx::lang::IllegalStateException(
            "invalid serialized X.509 certificate length");
    const auto candidate = ::jxx::NEW<
        ::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>(length);
    input->readFully(candidate);
    validateX509Der(candidate);
    encoded_ = copyBytes(candidate);
}

void DerCertificate::readObjectNoData() {
    throw ::jxx::lang::IllegalStateException(
        "X.509 certificate requires serialized data");
}

} // namespace jxx::ext::net::ssl::internal
