#pragma once

#include "security/jxx.security.PrivateKey.h"

namespace jxx::security::internal {

class EncodedPrivateKey final
    : public ::jxx::lang::InterfaceBase<
          EncodedPrivateKey,
          ::jxx::security::PrivateKey> {
public:
    EncodedPrivateKey(
        const ::jxx::Ptr<::jxx::lang::String>& algorithm,
        const ::jxx::lang::ByteArray& encoded);
    ::jxx::Ptr<::jxx::lang::String> getAlgorithm() const override;
    ::jxx::Ptr<::jxx::lang::String> getFormat() const override;
    ::jxx::lang::ByteArray getEncoded() const override;
private:
    ::jxx::Ptr<::jxx::lang::String> algorithm_;
    ::jxx::lang::ByteArray encoded_;
};

} // namespace jxx::security::internal
