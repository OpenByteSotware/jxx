#include <gtest/gtest.h>
#include <type_traits>

#include "jxx.initialize.h"
#include "lang/management/jxx.lang.management.ClassLoadingMXBean.h"
#include "lang/management/jxx.lang.management.CompilationMXBean.h"
#include "lang/management/jxx.lang.management.OperatingSystemMXBean.h"
#include "lang/management/jxx.lang.management.PlatformManagedObject.h"

namespace {

TEST(JxxLangManagementCoreInterfacesTest, MatchesJava8Inheritance)
{
    using Platform = ::jxx::lang::management::PlatformManagedObject;
    EXPECT_TRUE((std::is_base_of_v<Platform,
        ::jxx::lang::management::ClassLoadingMXBean>));
    EXPECT_TRUE((std::is_base_of_v<Platform,
        ::jxx::lang::management::CompilationMXBean>));
    EXPECT_TRUE((std::is_base_of_v<Platform,
        ::jxx::lang::management::OperatingSystemMXBean>));
    EXPECT_FALSE((std::is_base_of_v<::jxx::lang::Object, Platform>));
}

TEST(JxxLangManagementCoreInterfacesTest, RegistersPublicInterfaces)
{
    ::jxx::initialize();
    EXPECT_NE(nullptr,
        ::jxx::lang::management::PlatformManagedObject::Class());
    EXPECT_NE(nullptr,
        ::jxx::lang::management::ClassLoadingMXBean::Class());
    EXPECT_NE(nullptr,
        ::jxx::lang::management::CompilationMXBean::Class());
    EXPECT_NE(nullptr,
        ::jxx::lang::management::OperatingSystemMXBean::Class());
}

} // namespace
