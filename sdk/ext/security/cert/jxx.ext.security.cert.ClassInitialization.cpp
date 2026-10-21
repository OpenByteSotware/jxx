#include "ext/security/cert/jxx.ext.security.cert.ClassInitialization.h"
#include <mutex>
#include "ext/security/cert/jxx.ext.security.cert.X509Certificate.h"
#include "ext/security/cert/internal/jxx.ext.security.cert.internal.ClassInitialization.h"
namespace jxx::ext::security::cert {
void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
        (void)::jxx::lang::ClassBase<
            X509Certificate,
            ::jxx::lang::Object>::Class();
    });
}

void initialize()
{
    initializeClasses();
    ::jxx::ext::security::cert::internal::initialize();
}

} // namespace jxx::ext::security::cert
