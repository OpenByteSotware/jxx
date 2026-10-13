#include "ext/security/cert/jxx.ext.security.cert.ClassInitialization.h"
#include <mutex>
#include "ext/security/cert/jxx.ext.security.cert.X509Certificate.h"
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
} // namespace jxx::ext::security::cert
