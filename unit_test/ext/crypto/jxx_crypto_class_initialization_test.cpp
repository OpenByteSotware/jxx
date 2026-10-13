#include <gtest/gtest.h>
#include "ext/crypto/jxx.ext.crypto.ClassInitialization.h"
#include "ext/crypto/jxx.ext.crypto.SecretKey.h"
#include "jxx.initialize.h"
namespace {
auto secretKeyClass()
{
    return ::jxx::lang::InterfaceBase<
        ::jxx::ext::crypto::SecretKey,
        ::jxx::security::Key,
        ::jxx::ext::security::auth::Destroyable>::Class();
}
}
TEST(JxxCryptoClassInitialization, SecretKeyHasClassMetadata)
{
    ::jxx::initialize();
    EXPECT_NE(secretKeyClass(), nullptr);
}
TEST(JxxCryptoClassInitialization, RepeatedInitializationIsIdempotent)
{
    const auto before = secretKeyClass();
    EXPECT_NO_THROW(::jxx::ext::crypto::initializeClasses());
    EXPECT_NO_THROW(::jxx::ext::crypto::initializeClasses());
    EXPECT_EQ(before, secretKeyClass());
}
