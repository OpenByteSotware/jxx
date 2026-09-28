#pragma once

#include "security/jxx.security.PublicKey.h"

namespace jxx::ext::net::ssl::internal {

class OpenSslPublicKey final
    : public ::jxx::lang::InterfaceBase<
          OpenSslPublicKey,
          ::jxx::security::PublicKey> {
public:
    OpenSslPublicKey(
        const ::jxx::Ptr<::jxx::lang::String>& algorithm,
        const ::jxx::lang::ByteArray& encoded);

    ::jxx::Ptr<::jxx::lang::String> getAlgorithm() const override;
    ::jxx::Ptr<::jxx::lang::String> getFormat() const override;
    ::jxx::lang::ByteArray getEncoded() const override;

private:
    ::jxx::Ptr<::jxx::lang::String> algorithm_;
    ::jxx::lang::ByteArray encoded_;
};

} // namespace jxx::ext::net::ssl::internal
