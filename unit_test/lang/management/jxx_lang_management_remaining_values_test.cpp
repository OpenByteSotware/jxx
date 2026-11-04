#include <gtest/gtest.h>
#include <type_traits>

#include "jxx.initialize.h"
#include "io/jxx.io.SerializationClassRegistry.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.String.h"
#include "lang/management/jxx.lang.management.ManagementPermission.h"
#include "lang/management/jxx.lang.management.MemoryNotificationInfo.h"
#include "lang/management/jxx.lang.management.ThreadInfo.h"
#include "security/jxx.security.BasicPermission.h"

namespace {

TEST(JxxLangManagementRemainingValuesTest, ManagementPermissionValidatesJava8Names)
{
    const auto monitor = ::jxx::NEW<::jxx::lang::management::ManagementPermission>(
        ::jxx::NEW<::jxx::lang::String>("monitor"));
    EXPECT_EQ("monitor", monitor->getName()->utf8());
    EXPECT_TRUE((std::is_base_of_v<::jxx::security::BasicPermission,
        ::jxx::lang::management::ManagementPermission>));
    EXPECT_THROW((::jxx::NEW<::jxx::lang::management::ManagementPermission>(
        ::jxx::NEW<::jxx::lang::String>("invalid"))),
        ::jxx::lang::IllegalArgumentException);
    EXPECT_THROW((::jxx::NEW<::jxx::lang::management::ManagementPermission>(
        ::jxx::NEW<::jxx::lang::String>("control"),
        ::jxx::NEW<::jxx::lang::String>("read"))),
        ::jxx::lang::IllegalArgumentException);
}

TEST(JxxLangManagementRemainingValuesTest, MemoryNotificationInfoStoresValues)
{
    const auto usage = ::jxx::NEW<::jxx::lang::management::MemoryUsage>(
        -1, 100, 200, 400);
    const auto info =
        ::jxx::NEW<::jxx::lang::management::MemoryNotificationInfo>(
            ::jxx::NEW<::jxx::lang::String>("heap"), usage, 7);
    EXPECT_EQ("heap", info->getPoolName()->utf8());
    EXPECT_EQ(usage, info->getUsage());
    EXPECT_EQ(7, info->getCount());
    EXPECT_EQ("java.management.memory.threshold.exceeded",
        ::jxx::lang::management::MemoryNotificationInfo::
            MEMORY_THRESHOLD_EXCEEDED->utf8());
}

TEST(JxxLangManagementRemainingValuesTest, FromNullMatchesJava8Contract)
{
    EXPECT_EQ(nullptr,
        ::jxx::lang::management::MemoryNotificationInfo::from(nullptr));
    EXPECT_EQ(nullptr, ::jxx::lang::management::ThreadInfo::from(nullptr));
}

TEST(JxxLangManagementRemainingValuesTest, InitializationRegistersNewTypes)
{
    ::jxx::initialize();
    EXPECT_NE(nullptr,
        ::jxx::lang::management::ManagementPermission::Class());
    EXPECT_NE(nullptr,
        ::jxx::lang::management::MemoryNotificationInfo::Class());
    EXPECT_NE(nullptr, ::jxx::lang::management::ThreadInfo::Class());
    const auto metadata =
        ::jxx::io::serialization::SerializationClassRegistry::resolve(
            ::jxx::lang::management::ManagementPermission::Class()
                ->getName()->utf8());
    EXPECT_EQ(static_cast<::jxx::lang::jlong>(1897496590799378737LL),
        metadata.serialVersionUID);
}

} // namespace
