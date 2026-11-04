#include "ext/security/cert/internal/jxx.ext.security.cert.internal.DerX509Certificate.h"

#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.UnsupportedOperationException.h"
#include "security/jxx.security.PublicKey.h"

namespace jxx::ext::security::cert::internal {

DerX509Certificate::DerX509Certificate(
    const ::jxx::lang::ByteArray& encoded)
    : Super()
    , encoded_(encoded) {
    if (encoded_ == nullptr)
        throw ::jxx::lang::NullPointerException();
}

::jxx::lang::ByteArray DerX509Certificate::getEncoded() const {
    const auto copy = ::jxx::NEW<
        ::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>(encoded_->length);
    for (::jxx::lang::jint index = 0; index < encoded_->length; ++index)
        (*copy)[index] = (*encoded_)[index];
    return copy;
}

void DerX509Certificate::verify(
    const ::jxx::Ptr<::jxx::security::PublicKey>&) const {
    throw ::jxx::lang::UnsupportedOperationException(
        "Certificate signature verification is not implemented");
}

void DerX509Certificate::verify(
    const ::jxx::Ptr<::jxx::security::PublicKey>&,
    const ::jxx::Ptr<::jxx::lang::String>&) const {
    throw ::jxx::lang::UnsupportedOperationException(
        "Certificate signature verification is not implemented");
}

::jxx::Ptr<::jxx::security::PublicKey>
DerX509Certificate::getPublicKey() const {
    return nullptr;
}

::jxx::Ptr<::jxx::lang::String> DerX509Certificate::toString() const {
    return ::jxx::NEW<::jxx::lang::String>("X.509 certificate");
}

} // namespace jxx::ext::security::cert::internal
