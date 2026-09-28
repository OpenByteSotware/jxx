#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslPublicKey.h"

#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::ext::net::ssl::internal {

OpenSslPublicKey::OpenSslPublicKey(
    const ::jxx::Ptr<::jxx::lang::String>& algorithm,
    const ::jxx::lang::ByteArray& encoded)
    : algorithm_(algorithm)
    , encoded_(encoded) {
    if (algorithm_ == nullptr || encoded_ == nullptr)
        throw ::jxx::lang::NullPointerException();
}

::jxx::Ptr<::jxx::lang::String>
OpenSslPublicKey::getAlgorithm() const { return algorithm_; }

::jxx::Ptr<::jxx::lang::String>
OpenSslPublicKey::getFormat() const {
    return ::jxx::NEW<::jxx::lang::String>("X.509");
}

::jxx::lang::ByteArray OpenSslPublicKey::getEncoded() const {
    const auto copy = ::jxx::NEW<
        ::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>(encoded_->length);
    for (::jxx::lang::jint i = 0; i < encoded_->length; ++i)
        (*copy)[i] = (*encoded_)[i];
    return copy;
}

} // namespace jxx::ext::net::ssl::internal
