#include "security/internal/jxx.security.internal.EncodedPrivateKey.h"

namespace jxx::security::internal {

EncodedPrivateKey::EncodedPrivateKey(
    const ::jxx::Ptr<::jxx::lang::String>& algorithm,
    const ::jxx::lang::ByteArray& encoded)
    : algorithm_(algorithm), encoded_(encoded) {}

::jxx::Ptr<::jxx::lang::String> EncodedPrivateKey::getAlgorithm() const { return algorithm_; }
::jxx::Ptr<::jxx::lang::String> EncodedPrivateKey::getFormat() const {
    return ::jxx::NEW<::jxx::lang::String>("PKCS#8");
}
::jxx::lang::ByteArray EncodedPrivateKey::getEncoded() const {
    const auto copy = ::jxx::NEW<::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>(encoded_->length);
    for (::jxx::lang::jint i = 0; i < encoded_->length; ++i) (*copy)[i] = (*encoded_)[i];
    return copy;
}

} // namespace jxx::security::internal
