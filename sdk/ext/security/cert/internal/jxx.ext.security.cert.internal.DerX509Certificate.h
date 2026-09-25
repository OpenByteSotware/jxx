#pragma once

#include "ext/security/cert/jxx.ext.security.cert.X509Certificate.h"

namespace jxx::ext::security::cert::internal {

class DerX509Certificate final
    : public ::jxx::lang::ClassBase<
          DerX509Certificate,
          ::jxx::ext::security::cert::X509Certificate> {
public:
    using JxxSuper = ::jxx::ext::security::cert::X509Certificate;
    using Super = ::jxx::lang::ClassBase<DerX509Certificate, JxxSuper>;

    explicit DerX509Certificate(const ::jxx::lang::ByteArray& encoded);
    ::jxx::lang::ByteArray getEncoded() const override;

private:
    ::jxx::lang::ByteArray encoded_;
};

} // namespace jxx::ext::security::cert::internal
