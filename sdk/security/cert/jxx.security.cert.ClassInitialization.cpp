#include "security/cert/jxx.security.cert.ClassInitialization.h"

#include <mutex>

#include "security/cert/jxx.security.cert.CertPathParameters.h"
#include "security/cert/jxx.security.cert.Certificate.h"
#include "security/cert/jxx.security.cert.X509Certificate.h"

namespace jxx::security::cert {

void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
        (void)::jxx::lang::InterfaceBase<CertPathParameters>::Class();
        (void)::jxx::lang::ClassBase<
            Certificate,
            ::jxx::lang::Object,
            ::jxx::io::SerializableI>::Class();
        (void)::jxx::lang::ClassBase<
            X509Certificate,
            Certificate>::Class();
    });
}


void initialize()
{
    initializeClasses();
}

} // namespace jxx::security::cert
