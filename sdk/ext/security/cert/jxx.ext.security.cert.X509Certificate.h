#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "ext/security/cert/jxx.ext.security.cert.Certificate.h"
#include "lang/jxx.lang.JxxArray.h"

namespace jxx::ext::security::cert {

class X509Certificate
    : public ::jxx::lang::ClassBase<
          X509Certificate,
          Certificate> {
public:
    using JxxSuper = Certificate;
    using Super = ::jxx::lang::ClassBase<X509Certificate, JxxSuper>;

    ~X509Certificate() override = default;
    virtual ::jxx::lang::ByteArray getEncoded() const = 0;

protected:
    X509Certificate() = default;
};

} // namespace jxx::ext::security::cert
