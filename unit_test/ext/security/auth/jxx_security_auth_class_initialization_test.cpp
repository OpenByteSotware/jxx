#include <gtest/gtest.h>
#include "ext/security/auth/jxx.ext.security.auth.ClassInitialization.h"
#include "ext/security/auth/jxx.ext.security.auth.Destroyable.h"
#include "jxx.initialize.h"
namespace {
auto destroyableClass()
{
    return ::jxx::lang::InterfaceBase<
        ::jxx::ext::security::auth::Destroyable>::Class();
}
}
TEST(JxxSecurityAuthClassInitialization, DestroyableHasClassMetadata)
{
    ::jxx::initialize();
    EXPECT_NE(destroyableClass(), nullptr);
}
TEST(JxxSecurityAuthClassInitialization, RepeatedInitializationIsIdempotent)
{
    const auto before = destroyableClass();
    EXPECT_NO_THROW(::jxx::ext::security::auth::initializeClasses());
    EXPECT_NO_THROW(::jxx::ext::security::auth::initializeClasses());
    EXPECT_EQ(before, destroyableClass());
}
