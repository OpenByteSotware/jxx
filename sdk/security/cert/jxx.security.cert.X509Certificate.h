#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "security/cert/jxx.security.cert.Certificate.h"
#include "security/jxx.security.Principal.h"

namespace jxx::security::cert {

class X509Certificate
    : public ::jxx::lang::ClassBase<
          X509Certificate,
          Certificate> {
public:
    using JxxSuper = Certificate;
    using Super =
        ::jxx::lang::ClassBase<
            X509Certificate,
            JxxSuper>;

    explicit X509Certificate(
        const ::jxx::Ptr<::jxx::lang::String>& type)
        : Super(type) {
    }

    ~X509Certificate() override = default;

    virtual void checkValidity() const = 0;

    virtual ::jxx::Ptr<::jxx::security::Principal>
    getIssuerDN() const = 0;

    virtual ::jxx::Ptr<::jxx::security::Principal>
    getSubjectDN() const = 0;
};

} // namespace jxx::security::cert
