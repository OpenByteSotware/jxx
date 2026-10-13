#include <gtest/gtest.h>
#include "ext/security/cert/jxx.ext.security.cert.ClassInitialization.h"
#include "ext/security/cert/jxx.ext.security.cert.X509Certificate.h"
#include "jxx.initialize.h"
namespace {
auto x509CertificateClass()
{
    return ::jxx::lang::ClassBase<
        ::jxx::ext::security::cert::X509Certificate,
        ::jxx::lang::Object>::Class();
}
}
TEST(JxxExtSecurityCertClassInitialization, X509CertificateHasClassMetadata)
{
    ::jxx::initialize();
    EXPECT_NE(x509CertificateClass(), nullptr);
}
TEST(JxxExtSecurityCertClassInitialization, RepeatedInitializationIsIdempotent)
{
    const auto before = x509CertificateClass();
    EXPECT_NO_THROW(::jxx::ext::security::cert::initializeClasses());
    EXPECT_NO_THROW(::jxx::ext::security::cert::initializeClasses());
    EXPECT_EQ(before, x509CertificateClass());
}
