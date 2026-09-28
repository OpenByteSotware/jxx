#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.DerCertificate.h"

#include <openssl/x509.h>

#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"

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
