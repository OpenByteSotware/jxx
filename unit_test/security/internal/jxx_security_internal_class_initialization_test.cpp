#include <gtest/gtest.h>

#include "jxx.initialize.h"
#include "security/internal/jxx.security.internal.ClassInitialization.h"
#include "security/internal/jxx.security.internal.EncodedPrivateKey.h"

namespace {

auto encodedPrivateKeyClass()
{
    return ::jxx::lang::InterfaceBase<
        ::jxx::security::internal::EncodedPrivateKey,
        ::jxx::security::PrivateKey>::Class();
}

} // namespace

TEST(JxxSecurityInternalClassInitialization, EncodedPrivateKeyHasClassMetadata)
{
    ::jxx::initialize();
    EXPECT_NE(encodedPrivateKeyClass(), nullptr);
}

TEST(JxxSecurityInternalClassInitialization, RepeatedInitializationIsIdempotent)
{
    const auto before = encodedPrivateKeyClass();

    EXPECT_NO_THROW(::jxx::security::internal::initializeClasses());
    EXPECT_NO_THROW(::jxx::security::internal::initializeClasses());

    EXPECT_EQ(before, encodedPrivateKeyClass());
}
