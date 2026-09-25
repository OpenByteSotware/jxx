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
