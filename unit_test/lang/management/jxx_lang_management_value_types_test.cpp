#include <gtest/gtest.h>
#include <type_traits>

#include "jxx.initialize.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.StackTraceElement.h"
#include "lang/jxx.lang.String.h"
#include "lang/management/jxx.lang.management.LockInfo.h"
#include "lang/management/jxx.lang.management.MemoryType.h"
#include "lang/management/jxx.lang.management.MemoryUsage.h"
#include "lang/management/jxx.lang.management.MonitorInfo.h"

namespace {

TEST(JxxLangManagementValueTypesTest, MemoryUsageValidatesAndReportsValues)
{
    const auto usage = ::jxx::NEW<::jxx::lang::management::MemoryUsage>(
        -1, 1024, 2048, 4096);
    EXPECT_EQ(-1, usage->getInit());
    EXPECT_EQ(1024, usage->getUsed());
    EXPECT_EQ(2048, usage->getCommitted());
    EXPECT_EQ(4096, usage->getMax());
    EXPECT_THROW((::jxx::NEW<::jxx::lang::management::MemoryUsage>(
        0, 4096, 2048, 8192)), ::jxx::lang::IllegalArgumentException);
}

TEST(JxxLangManagementValueTypesTest, LockInfoMatchesJavaTextForm)
{
    const auto lock = ::jxx::NEW<::jxx::lang::management::LockInfo>(
        ::jxx::NEW<::jxx::lang::String>("example.Lock"), 0x1234);
    EXPECT_EQ("example.Lock", lock->getClassName()->utf8());
    EXPECT_EQ(0x1234, lock->getIdentityHashCode());
    EXPECT_EQ("example.Lock@1234", lock->toString()->utf8());
}

TEST(JxxLangManagementValueTypesTest, MonitorInfoExtendsLockInfo)
{
    EXPECT_TRUE((std::is_base_of_v<::jxx::lang::management::LockInfo,
        ::jxx::lang::management::MonitorInfo>));
    const auto frame = ::jxx::NEW<::jxx::lang::StackTraceElement>(
        ::jxx::NEW<::jxx::lang::String>("Example"),
        ::jxx::NEW<::jxx::lang::String>("run"),
        ::jxx::NEW<::jxx::lang::String>("Example.cpp"), 42);
    const auto monitor = ::jxx::NEW<::jxx::lang::management::MonitorInfo>(
        ::jxx::NEW<::jxx::lang::String>("example.Lock"), 7, 3, frame);
    EXPECT_EQ(3, monitor->getLockedStackDepth());
    EXPECT_EQ(frame, monitor->getLockedStackFrame());
}

TEST(JxxLangManagementValueTypesTest, MemoryTypeHasJava8EnumBehavior)
{
    const auto values = ::jxx::lang::management::MemoryType::values();
    ASSERT_NE(nullptr, values);
    ASSERT_EQ(2, values->length);
    EXPECT_EQ(::jxx::lang::management::MemoryType::HEAP, (*values)[0]);
    EXPECT_EQ("Heap memory",
        ::jxx::lang::management::MemoryType::HEAP->toString()->utf8());
    EXPECT_EQ(::jxx::lang::management::MemoryType::NON_HEAP,
        ::jxx::lang::management::MemoryType::valueOf(
            ::jxx::NEW<::jxx::lang::String>("NON_HEAP")));
}

TEST(JxxLangManagementValueTypesTest, GlobalInitializationRegistersTypes)
{
    ::jxx::initialize();
    EXPECT_NE(nullptr, ::jxx::lang::management::MemoryUsage::Class());
    EXPECT_NE(nullptr, ::jxx::lang::management::LockInfo::Class());
    EXPECT_NE(nullptr, ::jxx::lang::management::MonitorInfo::Class());
    EXPECT_NE(nullptr, ::jxx::lang::management::MemoryType::Class());
}

} // namespace
