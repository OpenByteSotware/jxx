#include "security/cert/jxx.security.cert.ClassInitialization.h"

#include <mutex>

#include "security/cert/jxx.security.cert.CertPathParameters.h"
#include "security/cert/jxx.security.cert.Certificate.h"
#include "security/cert/jxx.security.cert.X509Certificate.h"

#include "security/cert/jxx.security.cert.X509Extension.h"
namespace jxx::security::cert {

void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
        ::jxx::lang::class_info_detail::registerClassInfo<X509Extension>();
            ::jxx::lang::class_info_detail::registerClassInfo<CertPathParameters>();
            ::jxx::lang::class_info_detail::registerClassInfo<Certificate>();
            ::jxx::lang::class_info_detail::registerClassInfo<X509Certificate>();
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
