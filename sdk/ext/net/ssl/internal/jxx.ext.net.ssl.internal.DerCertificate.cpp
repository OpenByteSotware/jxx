#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.DerCertificate.h"

#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.UnsupportedOperationException.h"

namespace jxx::ext::net::ssl::internal {

DerCertificate::DerCertificate(
    const ::jxx::lang::ByteArray& encoded)
    : Super(::jxx::NEW<::jxx::lang::String>("X.509"))
    , encoded_(encoded) {
    if (encoded_ == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
}

::jxx::lang::ByteArray DerCertificate::getEncoded() const {
    const auto copy =
        ::jxx::NEW<
            ::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>(
                encoded_->length);

    for (::jxx::lang::jint index = 0;
         index < encoded_->length;
         ++index) {
        (*copy)[index] = (*encoded_)[index];
    }

    return copy;
}

void DerCertificate::writeObject(
    const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& output) {
    (void)output;
    throw ::jxx::lang::UnsupportedOperationException(
        ::jxx::NEW<::jxx::lang::String>(
            "DerCertificate object serialization is not implemented"));
}

void DerCertificate::readObject(
    const ::jxx::Ptr<::jxx::io::ObjectInputStream>& input) {
    (void)input;
    throw ::jxx::lang::UnsupportedOperationException(
        ::jxx::NEW<::jxx::lang::String>(
            "DerCertificate object deserialization is not implemented"));
}

void DerCertificate::readObjectNoData() {
    throw ::jxx::lang::UnsupportedOperationException(
        ::jxx::NEW<::jxx::lang::String>(
            "DerCertificate object deserialization is not implemented"));
}

} // namespace jxx::ext::net::ssl::internal
