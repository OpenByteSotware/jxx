#include "ext/security/auth/jxx.ext.security.auth.ClassInitialization.h"
#include <mutex>
#include "ext/security/auth/jxx.ext.security.auth.Destroyable.h"
#include "ext/security/auth/x500/jxx.ext.security.auth.x500.ClassInitialization.h"
namespace jxx::ext::security::auth {
void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
        (void)::jxx::lang::InterfaceBase<Destroyable>::Class();
    });
}

void initialize()
{
    initializeClasses();
    ::jxx::ext::security::auth::x500::initialize();
}

} // namespace jxx::ext::security::auth
