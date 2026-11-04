#include "ext/security/cert/jxx.ext.security.cert.ClassInitialization.h"
#include <mutex>
#include "ext/security/cert/jxx.ext.security.cert.Certificate.h"
#include "ext/security/cert/jxx.ext.security.cert.X509Certificate.h"
#include "ext/security/cert/internal/jxx.ext.security.cert.internal.ClassInitialization.h"
namespace jxx::ext::security::cert {
void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
        (void)Certificate::Class();
        (void)X509Certificate::Class();
    });
}

void initialize()
{
    initializeClasses();
    ::jxx::ext::security::cert::internal::initialize();
}

} // namespace jxx::ext::security::cert
