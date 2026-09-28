#pragma once

#include "security/jxx.security.BasicPermission.h"

namespace jxx::ext::net::ssl {

class SSLPermission final
    : public ::jxx::lang::ClassBase<
          SSLPermission,
          ::jxx::security::BasicPermission> {
public:
    using JxxSuper = ::jxx::security::BasicPermission;
    using Super = ::jxx::lang::ClassBase<SSLPermission, JxxSuper>;

    explicit SSLPermission(
        const ::jxx::Ptr<::jxx::lang::String>& name);

    SSLPermission(
        const ::jxx::Ptr<::jxx::lang::String>& name,
        const ::jxx::Ptr<::jxx::lang::String>& actions);

    ~SSLPermission() override = default;

protected:
    ::jxx::Ptr<::jxx::lang::Object>
    cloneImpl() const override;

};

} // namespace jxx::ext::net::ssl
