#include <gtest/gtest.h>

#include "jxx.initialize.h"
#include "security/cert/jxx.security.cert.CertPathParameters.h"
#include "security/cert/jxx.security.cert.Certificate.h"
#include "security/cert/jxx.security.cert.ClassInitialization.h"
#include "security/cert/jxx.security.cert.X509Certificate.h"

TEST(JxxSecurityCertClassInitialization, MetadataTypesAreInitialized)
{
    namespace C = ::jxx::security::cert;
    ::jxx::initialize();

    EXPECT_NE((::jxx::lang::InterfaceBase<C::CertPathParameters>::Class()), nullptr);
    EXPECT_NE((::jxx::lang::ClassBase<C::Certificate, ::jxx::lang::Object, ::jxx::io::SerializableI>::Class()), nullptr);
    EXPECT_NE((::jxx::lang::ClassBase<C::X509Certificate, C::Certificate>::Class()), nullptr);
}

TEST(JxxSecurityCertClassInitialization, RepeatedInitializationIsIdempotent)
{
    namespace C = ::jxx::security::cert;
    const auto before = ::jxx::lang::ClassBase<C::X509Certificate, C::Certificate>::Class();

    EXPECT_NO_THROW(C::initializeClasses());
    EXPECT_NO_THROW(C::initializeClasses());

    EXPECT_EQ(before, (::jxx::lang::ClassBase<C::X509Certificate, C::Certificate>::Class()));
}
