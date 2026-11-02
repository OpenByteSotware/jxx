#include <gtest/gtest.h>
#include <type_traits>

#include "security/jxx.security.ClassInitialization.h"
#include "security/jxx.security.GeneralSecurityException.h"
#include "security/jxx.security.KeyException.h"
#include "security/jxx.security.KeyManagementException.h"
#include "security/jxx.security.UnrecoverableEntryException.h"
#include "security/jxx.security.UnrecoverableKeyException.h"

TEST(SecurityExceptionInheritanceParity, KeyHierarchyMatchesSource) {
    static_assert(std::is_base_of_v<
        ::jxx::security::GeneralSecurityException,
        ::jxx::security::KeyException>);
    static_assert(std::is_base_of_v<
        ::jxx::security::KeyException,
        ::jxx::security::KeyManagementException>);
    SUCCEED();
}

TEST(SecurityExceptionInheritanceParity, UnrecoverableHierarchyMatchesSource) {
    static_assert(std::is_base_of_v<
        ::jxx::security::GeneralSecurityException,
        ::jxx::security::UnrecoverableEntryException>);
    static_assert(std::is_base_of_v<
        ::jxx::security::UnrecoverableEntryException,
        ::jxx::security::UnrecoverableKeyException>);
    SUCCEED();
}

TEST(SecurityExceptionInheritanceParity, PackageInitializationRegistersCorrectMetadata) {
    ::jxx::security::initializeClasses();

    EXPECT_NE(nullptr, ::jxx::security::KeyException::Class());
    EXPECT_NE(nullptr, ::jxx::security::KeyManagementException::Class());
    EXPECT_NE(nullptr, ::jxx::security::UnrecoverableEntryException::Class());
    EXPECT_NE(nullptr, ::jxx::security::UnrecoverableKeyException::Class());
}
