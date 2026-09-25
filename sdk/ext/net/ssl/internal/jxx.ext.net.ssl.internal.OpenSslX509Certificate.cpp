#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslX509Certificate.h"

#include <openssl/x509.h>

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
