#include <gtest/gtest.h>

#include "ext/security/cert/internal/jxx.ext.security.cert.internal.ClassInitialization.h"
#include "ext/security/cert/internal/jxx.ext.security.cert.internal.DerX509Certificate.h"
#include "jxx.initialize.h"

namespace {

auto derX509CertificateClass()
{
    return ::jxx::lang::ClassBase<
        ::jxx::ext::security::cert::internal::DerX509Certificate,
        ::jxx::ext::security::cert::X509Certificate>::Class();
}

} // namespace

TEST(JxxExtSecurityCertInternalClassInitialization, DerX509CertificateHasMetadata)
{
    ::jxx::initialize();

    EXPECT_NE(derX509CertificateClass(), nullptr);
}

TEST(JxxExtSecurityCertInternalClassInitialization, RepeatedInitializationIsIdempotent)
{
    const auto before = derX509CertificateClass();

    EXPECT_NO_THROW(
        ::jxx::ext::security::cert::internal::initializeClasses());
    EXPECT_NO_THROW(
        ::jxx::ext::security::cert::internal::initializeClasses());

    EXPECT_EQ(before, derX509CertificateClass());
}
