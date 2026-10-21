#include "ext/security/auth/x500/jxx.ext.security.auth.x500.ClassInitialization.h"
#include <mutex>
#include "ext/security/auth/x500/jxx.ext.security.auth.x500.X500Principal.h"
namespace jxx::ext::security::auth::x500 {
void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
        (void)::jxx::lang::ClassBase<
            X500Principal,
            ::jxx::lang::Object,
            ::jxx::security::Principal>::Class();
    });
}

void initialize()
{
    initializeClasses();
}

} // namespace jxx::ext::security::auth::x500
