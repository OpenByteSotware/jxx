#pragma once

#include "security/jxx.security.Permission.h"

namespace jxx::security {

class BasicPermission
    : public ::jxx::lang::ClassBase<BasicPermission, Permission> {
public:
    using JxxSuper = Permission;
    using Super = ::jxx::lang::ClassBase<BasicPermission, JxxSuper>;

    explicit BasicPermission(const ::jxx::Ptr<::jxx::lang::String>& name);
    BasicPermission(
        const ::jxx::Ptr<::jxx::lang::String>& name,
        const ::jxx::Ptr<::jxx::lang::String>& actions);

    ::jxx::lang::jbool implies(
        const ::jxx::Ptr<Permission>& permission) const override;
    ::jxx::lang::jbool equals(
        const ::jxx::Ptr<::jxx::lang::Object>& object) const override;
    ::jxx::lang::jint hashCode() const override;
    ::jxx::Ptr<::jxx::lang::String> getActions() const override;

protected:
    ::jxx::lang::jbool wildcard_ = false;
    ::jxx::Ptr<::jxx::lang::String> path_;
};

} // namespace jxx::security
