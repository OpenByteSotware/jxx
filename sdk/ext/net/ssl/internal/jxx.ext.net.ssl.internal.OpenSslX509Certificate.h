#pragma once

#include "security/cert/jxx.security.cert.X509Certificate.h"

namespace jxx::io {
class ObjectInputStream;
class ObjectOutputStream;
}

namespace jxx::ext::net::ssl::internal {

class OpenSslX509Certificate final
    : public ::jxx::lang::ClassBase<
          OpenSslX509Certificate,
          ::jxx::security::cert::X509Certificate> {
public:
    using JxxSuper =
        ::jxx::security::cert::X509Certificate;
    using Super =
        ::jxx::lang::ClassBase<
            OpenSslX509Certificate,
            JxxSuper>;

    explicit OpenSslX509Certificate(
        const ::jxx::lang::ByteArray& encoded);

    ::jxx::lang::ByteArray getEncoded() const override;
    void checkValidity() const override;
    ::jxx::lang::jint getVersion() const override;
    ::jxx::Ptr<::jxx::math::BigInteger>
    getSerialNumber() const override;
    ::jxx::Ptr<::jxx::util::Date> getNotBefore() const override;
    ::jxx::Ptr<::jxx::util::Date> getNotAfter() const override;
    ::jxx::lang::ByteArray getTBSCertificate() const override;
    ::jxx::lang::ByteArray getSignature() const override;
    ::jxx::Ptr<::jxx::lang::String> getSigAlgName() const override;
    ::jxx::Ptr<::jxx::lang::String> getSigAlgOID() const override;
    ::jxx::lang::ByteArray getSigAlgParams() const override;
    ::jxx::Ptr<::jxx::security::PublicKey>
    getPublicKey() const override;
    ::jxx::lang::BooleanArray getIssuerUniqueID() const override;
    ::jxx::lang::BooleanArray getSubjectUniqueID() const override;
    ::jxx::lang::BooleanArray getKeyUsage() const override;
    ::jxx::lang::jint getBasicConstraints() const override;
    ::jxx::Ptr<::jxx::util::List<::jxx::lang::String>>
    getExtendedKeyUsage() const override;
    ::jxx::Ptr<::jxx::util::Set<::jxx::lang::String>>
    getCriticalExtensionOIDs() const override;
    ::jxx::Ptr<::jxx::util::Set<::jxx::lang::String>>
    getNonCriticalExtensionOIDs() const override;
    ::jxx::lang::ByteArray getExtensionValue(
        const ::jxx::Ptr<::jxx::lang::String>& oid) const override;
    ::jxx::lang::jbool hasUnsupportedCriticalExtension() const override;

    ::jxx::Ptr<
        ::jxx::ext::security::auth::x500::X500Principal>
    getIssuerX500Principal() const override;

    ::jxx::Ptr<
        ::jxx::ext::security::auth::x500::X500Principal>
    getSubjectX500Principal() const override;

    ::jxx::Ptr<::jxx::security::Principal>
    getIssuerDN() const override;

    ::jxx::Ptr<::jxx::security::Principal>
    getSubjectDN() const override;

    void writeObject(
        const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& output) override;

    void readObject(
        const ::jxx::Ptr<::jxx::io::ObjectInputStream>& input) override;

    void readObjectNoData() override;

private:
    ::jxx::lang::ByteArray encoded_;
};

} // namespace jxx::ext::net::ssl::internal
