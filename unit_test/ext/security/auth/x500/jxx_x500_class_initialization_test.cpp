#include <gtest/gtest.h>
#include "ext/security/auth/x500/jxx.ext.security.auth.x500.ClassInitialization.h"
#include "ext/security/auth/x500/jxx.ext.security.auth.x500.X500Principal.h"
#include "jxx.initialize.h"
namespace {
auto x500PrincipalClass()
{
    return ::jxx::lang::ClassBase<
        ::jxx::ext::security::auth::x500::X500Principal,
        ::jxx::lang::Object,
        ::jxx::security::Principal>::Class();
}
}
TEST(JxxX500ClassInitialization, X500PrincipalHasClassMetadata)
{
    ::jxx::initialize();
    EXPECT_NE(x500PrincipalClass(), nullptr);
}
TEST(JxxX500ClassInitialization, RepeatedInitializationIsIdempotent)
{
    const auto before = x500PrincipalClass();
    EXPECT_NO_THROW(::jxx::ext::security::auth::x500::initializeClasses());
    EXPECT_NO_THROW(::jxx::ext::security::auth::x500::initializeClasses());
    EXPECT_EQ(before, x500PrincipalClass());
}
