#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.ThreadGroup.h"

namespace {
using ::jxx::lang::String;
using ::jxx::lang::ThreadGroup;

TEST(ThreadGroupDeprecatedParityTest, EmptyGroupOperationsAreCallable) {
    const auto group = ::jxx::NEW<ThreadGroup>(
        ::jxx::NEW<String>("deprecated-operations"));
    group->suspend();
    group->resume();
    group->stop();
    group->destroy();
    EXPECT_TRUE(group->isDestroyed());
}

TEST(ThreadGroupSecurityParityTest, CheckAccessIsCallableWithoutManager) {
    const auto group = ::jxx::NEW<ThreadGroup>(
        ::jxx::NEW<String>("access"));
    group->checkAccess();
    group->destroy();
}

} // namespace
