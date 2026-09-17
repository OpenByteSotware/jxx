#pragma once

#include "security/jxx.security.BasicPermission.h"

namespace jxx::lang {

class RuntimePermission final
    : public ::jxx::lang::ClassBase<
          RuntimePermission,
          ::jxx::security::BasicPermission> {
public:
    using JxxSuper = ::jxx::security::BasicPermission;
    using Super = ::jxx::lang::ClassBase<RuntimePermission, JxxSuper>;
    using JxxClassInfoMarker = ::jxx::lang::ClassInfo<RuntimePermission, JxxSuper>;

    static ::jxx::Ptr<ClassAny> Class();
    explicit RuntimePermission(const ::jxx::Ptr<String>& name);
    RuntimePermission(
        const ::jxx::Ptr<String>& name,
        const ::jxx::Ptr<String>& actions);
};

} // namespace jxx::lang
