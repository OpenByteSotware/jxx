#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "security/cert/jxx.security.cert.Certificate.h"
#include "security/jxx.security.Principal.h"
#include "math/jxx.math.BigInteger.h"
#include "security/jxx.security.PublicKey.h"
#include "util/jxx.util.Date.h"
#include "util/jxx.util.Set.h"

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

    virtual ::jxx::lang::jint getVersion() const = 0;
    virtual ::jxx::Ptr<::jxx::math::BigInteger>
    getSerialNumber() const = 0;
    virtual ::jxx::Ptr<::jxx::util::Date> getNotBefore() const = 0;
    virtual ::jxx::Ptr<::jxx::util::Date> getNotAfter() const = 0;
    virtual ::jxx::lang::ByteArray getTBSCertificate() const = 0;
    virtual ::jxx::lang::ByteArray getSignature() const = 0;
    virtual ::jxx::Ptr<::jxx::lang::String> getSigAlgName() const = 0;
    virtual ::jxx::Ptr<::jxx::lang::String> getSigAlgOID() const = 0;
    virtual ::jxx::lang::ByteArray getSigAlgParams() const = 0;
    virtual ::jxx::Ptr<::jxx::security::PublicKey>
    getPublicKey() const = 0;
    virtual ::jxx::lang::BooleanArray getIssuerUniqueID() const = 0;
    virtual ::jxx::lang::BooleanArray getSubjectUniqueID() const = 0;
    virtual ::jxx::lang::BooleanArray getKeyUsage() const = 0;
    virtual ::jxx::lang::jint getBasicConstraints() const = 0;
    virtual ::jxx::Ptr<::jxx::util::Set<::jxx::lang::String>>
    getCriticalExtensionOIDs() const = 0;
    virtual ::jxx::Ptr<::jxx::util::Set<::jxx::lang::String>>
    getNonCriticalExtensionOIDs() const = 0;
    virtual ::jxx::lang::ByteArray getExtensionValue(
        const ::jxx::Ptr<::jxx::lang::String>& oid) const = 0;
    virtual ::jxx::lang::jbool hasUnsupportedCriticalExtension() const = 0;

    virtual ::jxx::Ptr<::jxx::security::Principal>
    getIssuerDN() const = 0;

    virtual ::jxx::Ptr<::jxx::security::Principal>
    getSubjectDN() const = 0;
};

} // namespace jxx::security::cert
