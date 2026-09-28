#pragma once

#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"

namespace jxx::security {
class Provider;
class PublicKey;
}

namespace jxx::security::cert {

class Certificate
    : public ::jxx::lang::ClassBase<
          Certificate,
          ::jxx::lang::Object,
          ::jxx::io::SerializableI> {
public:
    explicit Certificate(
        const ::jxx::Ptr<::jxx::lang::String>& type);
    ~Certificate() override = default;

    ::jxx::Ptr<::jxx::lang::String> getType() const;
    virtual ::jxx::lang::ByteArray getEncoded() const = 0;
    virtual ::jxx::Ptr<::jxx::security::PublicKey>
    getPublicKey() const = 0;

    virtual void verify(
        const ::jxx::Ptr<::jxx::security::PublicKey>& key) const;
    virtual void verify(
        const ::jxx::Ptr<::jxx::security::PublicKey>& key,
        const ::jxx::Ptr<::jxx::lang::String>& provider) const;
    virtual void verify(
        const ::jxx::Ptr<::jxx::security::PublicKey>& key,
        const ::jxx::Ptr<::jxx::security::Provider>& provider) const;

    ::jxx::lang::jbool equals(
        const ::jxx::Ptr<::jxx::lang::Object>& other) const override;
    ::jxx::lang::jint hashCode() const override;
    ::jxx::Ptr<::jxx::lang::String> toString() const override;

private:
    ::jxx::Ptr<::jxx::lang::String> type_;
};

} // namespace jxx::security::cert
