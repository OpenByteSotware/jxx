#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "security/jxx.security.Principal.h"

namespace jxx::ext::net::ssl::internal {
class X509Principal final
    : public ::jxx::lang::ClassBase<
          X509Principal,
          ::jxx::lang::Object,
          ::jxx::security::Principal> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<
        X509Principal, JxxSuper, ::jxx::security::Principal>;
    explicit X509Principal(const ::jxx::Ptr<::jxx::lang::String>& name);
    ::jxx::Ptr<::jxx::lang::String> getName() const override;
    ::jxx::Ptr<::jxx::lang::String> toString() const override;
    ::jxx::lang::jbool equals(
        const ::jxx::Ptr<::jxx::lang::Object>& other) const override;
    ::jxx::lang::jint hashCode() const override;
private:
    ::jxx::Ptr<::jxx::lang::String> name_;
};
} // namespace jxx::ext::net::ssl::internal
