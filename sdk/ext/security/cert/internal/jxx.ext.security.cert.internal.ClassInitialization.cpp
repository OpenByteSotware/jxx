#include "ext/security/cert/internal/jxx.ext.security.cert.internal.ClassInitialization.h"

#include <mutex>

#include "ext/security/cert/internal/jxx.ext.security.cert.internal.DerX509Certificate.h"

namespace jxx::ext::security::cert::internal {

void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
        (void)::jxx::lang::ClassBase<
            DerX509Certificate,
            ::jxx::ext::security::cert::X509Certificate>::Class();
    });
}


void initialize()
{
    initializeClasses();
}

} // namespace jxx::ext::security::cert::internal
