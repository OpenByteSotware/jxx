#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslX509Certificate.h"

#include <openssl/x509.h>

#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.X509Principal.h"
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

::jxx::Ptr<::jxx::security::Principal> distinguishedName(
    const ::jxx::lang::ByteArray& encoded,
    bool issuer) {
    const unsigned char* cursor =
        reinterpret_cast<const unsigned char*>(&(*encoded)[0]);
    X509* certificate = d2i_X509(nullptr, &cursor, encoded->length);
    if (certificate == nullptr)
        throw ::jxx::io::IOException("Invalid DER X.509 certificate");

    X509_NAME* name = issuer
        ? X509_get_issuer_name(certificate)
        : X509_get_subject_name(certificate);
    BIO* memory = BIO_new(BIO_s_mem());
    if (name == nullptr || memory == nullptr) {
        if (memory != nullptr) BIO_free(memory);
        X509_free(certificate);
        throw ::jxx::io::IOException("Unable to read X.509 distinguished name");
    }

    const unsigned long flags =
        XN_FLAG_RFC2253 & ~ASN1_STRFLGS_ESC_MSB;
    if (X509_NAME_print_ex(memory, name, 0, flags) < 0) {
        BIO_free(memory);
        X509_free(certificate);
        throw ::jxx::io::IOException("Unable to format X.509 distinguished name");
    }

    char* data = nullptr;
    const long length = BIO_get_mem_data(memory, &data);
    const std::string value =
        data == nullptr || length <= 0
            ? std::string()
            : std::string(data, static_cast<std::size_t>(length));
    BIO_free(memory);
    X509_free(certificate);
    return ::jxx::NEW<X509Principal>(
        ::jxx::NEW<::jxx::lang::String>(value));
}

} // namespace

::jxx::Ptr<::jxx::security::Principal>
OpenSslX509Certificate::getIssuerDN() const {
    return distinguishedName(encoded_, true);
}

::jxx::Ptr<::jxx::security::Principal>
OpenSslX509Certificate::getSubjectDN() const {
    return distinguishedName(encoded_, false);
}

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
