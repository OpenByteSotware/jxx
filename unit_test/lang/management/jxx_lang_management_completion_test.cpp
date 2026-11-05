#include <gtest/gtest.h>
#include <type_traits>
#include "jxx.initialize.h"
#include "lang/jxx.lang.UnsupportedOperationException.h"
#include "lang/management/jxx.lang.management.ManagementFactory.h"
namespace {
TEST(JxxLangManagementCompletionTest, FinalTypesMatchHierarchy){
 using Platform=::jxx::lang::management::PlatformManagedObject;
 EXPECT_TRUE((std::is_base_of_v<Platform,::jxx::lang::management::RuntimeMXBean>));
 EXPECT_TRUE((std::is_base_of_v<Platform,::jxx::lang::management::ThreadMXBean>));
 EXPECT_TRUE((std::is_base_of_v<::jxx::lang::Object,::jxx::lang::management::ManagementFactory>));
}
TEST(JxxLangManagementCompletionTest, RegistersFinalPublicTypes){
 ::jxx::initialize();
 EXPECT_NE(nullptr,::jxx::lang::management::RuntimeMXBean::Class());
 EXPECT_NE(nullptr,::jxx::lang::management::ThreadMXBean::Class());
 EXPECT_NE(nullptr,::jxx::lang::management::ManagementFactory::Class());
 EXPECT_EQ("java.lang:type=Runtime",::jxx::lang::management::ManagementFactory::RUNTIME_MXBEAN_NAME->utf8());
}
TEST(JxxLangManagementCompletionTest, UnimplementedBackendFailsExplicitly){
 EXPECT_THROW((void)::jxx::lang::management::ManagementFactory::getRuntimeMXBean(),::jxx::lang::UnsupportedOperationException);
}
}
