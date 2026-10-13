#include "security/internal/jxx.security.internal.ClassInitialization.h"

#include <mutex>

#include "security/internal/jxx.security.internal.EncodedPrivateKey.h"

namespace jxx::security::internal {

void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
        (void)::jxx::lang::InterfaceBase<
            EncodedPrivateKey,
            ::jxx::security::PrivateKey>::Class();
    });
}

} // namespace jxx::security::internal
