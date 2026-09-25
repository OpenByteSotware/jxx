#pragma once
#include "security/cert/jxx.security.cert.Certificate.h"
namespace jxx::security::cert {
class X509Certificate : public ::jxx::lang::ClassBase<X509Certificate,Certificate> {
public:
    using JxxSuper = Certificate;
    using Super = ::jxx::lang::ClassBase<X509Certificate,JxxSuper>;
    using Certificate::Certificate;
    ~X509Certificate() override = default;
    virtual void checkValidity() const = 0;
    virtual ::jxx::Ptr<::jxx::security::Principal> getIssuerDN() const = 0;
    virtual ::jxx::Ptr<::jxx::security::Principal> getSubjectDN() const = 0;
};
}
