#pragma once

#include "security/jxx.security.BasicPermission.h"

namespace jxx::lang::management {

class ManagementPermission final
    : public ::jxx::lang::ClassBase<
          ManagementPermission, ::jxx::security::BasicPermission> {
public:
    using JxxSuper = ::jxx::security::BasicPermission;
    using Super = ::jxx::lang::ClassBase<ManagementPermission, JxxSuper>;

    explicit ManagementPermission(
        const ::jxx::Ptr<::jxx::lang::String>& name);
    ManagementPermission(
        const ::jxx::Ptr<::jxx::lang::String>& name,
        const ::jxx::Ptr<::jxx::lang::String>& actions);
};

} // namespace jxx::lang::management
