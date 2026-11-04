#include <gtest/gtest.h>
#include <type_traits>
#include "jxx.initialize.h"
#include "lang/management/jxx.lang.management.BufferPoolMXBean.h"
#include "lang/management/jxx.lang.management.GarbageCollectorMXBean.h"
#include "lang/management/jxx.lang.management.MemoryMXBean.h"
#include "lang/management/jxx.lang.management.MemoryPoolMXBean.h"
#include "lang/management/jxx.lang.management.PlatformLoggingMXBean.h"
namespace {
TEST(JxxLangManagementResourceInterfacesTest, MatchesJava8Inheritance) {
 using Platform=::jxx::lang::management::PlatformManagedObject;
 EXPECT_TRUE((std::is_base_of_v<Platform,::jxx::lang::management::BufferPoolMXBean>));
 EXPECT_TRUE((std::is_base_of_v<::jxx::lang::management::MemoryManagerMXBean,::jxx::lang::management::GarbageCollectorMXBean>));
 EXPECT_TRUE((std::is_base_of_v<Platform,::jxx::lang::management::MemoryMXBean>));
 EXPECT_TRUE((std::is_base_of_v<Platform,::jxx::lang::management::MemoryPoolMXBean>));
 EXPECT_TRUE((std::is_base_of_v<Platform,::jxx::lang::management::PlatformLoggingMXBean>));
}
TEST(JxxLangManagementResourceInterfacesTest, RegistersInterfaces) {
 ::jxx::initialize();
 EXPECT_NE(nullptr,::jxx::lang::management::BufferPoolMXBean::Class());
 EXPECT_NE(nullptr,::jxx::lang::management::MemoryManagerMXBean::Class());
 EXPECT_NE(nullptr,::jxx::lang::management::GarbageCollectorMXBean::Class());
 EXPECT_NE(nullptr,::jxx::lang::management::MemoryMXBean::Class());
 EXPECT_NE(nullptr,::jxx::lang::management::MemoryPoolMXBean::Class());
 EXPECT_NE(nullptr,::jxx::lang::management::PlatformLoggingMXBean::Class());
}
}
