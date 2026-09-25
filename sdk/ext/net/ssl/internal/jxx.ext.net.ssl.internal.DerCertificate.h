#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.buildin_array.h"
#include "security/cert/jxx.security.cert.Certificate.h"

namespace jxx::io {
class ObjectInputStream;
class ObjectOutputStream;
}

namespace jxx::ext::net::ssl::internal {

class DerCertificate final
    : public ::jxx::lang::ClassBase<
          DerCertificate,
          ::jxx::security::cert::Certificate> {
public:
    using JxxSuper = ::jxx::security::cert::Certificate;
    using Super = ::jxx::lang::ClassBase<DerCertificate, JxxSuper>;

    explicit DerCertificate(
        const ::jxx::lang::ByteArray& encoded);

    ::jxx::lang::ByteArray getEncoded() const override;

    void writeObject(
        const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& output) override;

    void readObject(
        const ::jxx::Ptr<::jxx::io::ObjectInputStream>& input) override;

    void readObjectNoData() override;

private:
    ::jxx::lang::ByteArray encoded_;
};

} // namespace jxx::ext::net::ssl::internal
