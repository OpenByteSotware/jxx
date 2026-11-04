#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.JxxArray.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

namespace jxx::security {
class PublicKey;
}

namespace jxx::ext::security::cert {

class Certificate
    : public ::jxx::lang::ClassBase<
          Certificate,
          ::jxx::lang::Object> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<Certificate, JxxSuper>;

    ~Certificate() override = default;

    virtual ::jxx::lang::ByteArray getEncoded() const = 0;
    virtual void verify(
        const ::jxx::Ptr<::jxx::security::PublicKey>& key) const = 0;
    virtual void verify(
        const ::jxx::Ptr<::jxx::security::PublicKey>& key,
        const ::jxx::Ptr<::jxx::lang::String>& signatureProvider) const = 0;
    virtual ::jxx::Ptr<::jxx::security::PublicKey>
        getPublicKey() const = 0;
    virtual ::jxx::Ptr<::jxx::lang::String> toString() const override = 0;

protected:
    Certificate() = default;
};

} // namespace jxx::ext::security::cert
