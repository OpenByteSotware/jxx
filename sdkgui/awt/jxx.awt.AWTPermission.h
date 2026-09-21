#pragma once

#include "security/jxx.security.BasicPermission.h"

namespace jxx::awt
{
    class AWTPermission : public ::jxx::lang::ClassBase<AWTPermission,
        ::jxx::security::BasicPermission>
    {
    public:
        using JxxSuper = ::jxx::security::BasicPermission;
        using Super = ::jxx::lang::ClassBase<AWTPermission, JxxSuper>;

        explicit AWTPermission(
            const ::jxx::Ptr<::jxx::lang::String>& name);
        AWTPermission(const ::jxx::Ptr<::jxx::lang::String>& name,
            const ::jxx::Ptr<::jxx::lang::String>& actions);
    };
}
