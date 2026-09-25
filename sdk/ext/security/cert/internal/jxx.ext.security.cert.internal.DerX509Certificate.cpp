#include "ext/security/cert/internal/jxx.ext.security.cert.internal.DerX509Certificate.h"

#include "lang/jxx.lang.NullPointerException.h"

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

} // namespace jxx::ext::security::cert::internal
