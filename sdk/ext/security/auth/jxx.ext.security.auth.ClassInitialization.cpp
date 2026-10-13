#include "ext/security/auth/jxx.ext.security.auth.ClassInitialization.h"
#include <mutex>
#include "ext/security/auth/jxx.ext.security.auth.Destroyable.h"
namespace jxx::ext::security::auth {
void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
        (void)::jxx::lang::InterfaceBase<Destroyable>::Class();
    });
}
} // namespace jxx::ext::security::auth
