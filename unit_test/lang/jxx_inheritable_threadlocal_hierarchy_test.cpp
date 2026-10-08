#include <gtest/gtest.h>

#include "lang/jxx.lang.InheritableThreadLocal.h"
#include "lang/jxx.lang.String.h"

TEST(JxxInheritableThreadLocalHierarchyTest, HasDistinctClassMetadata) {
    using Inheritable =
        ::jxx::lang::InheritableThreadLocal<::jxx::lang::String>;
    using Base = ::jxx::lang::ThreadLocal<::jxx::lang::String>;

    EXPECT_NE(Inheritable::Class(), Base::Class());
}

TEST(JxxInheritableThreadLocalHierarchyTest, RetainsThreadLocalBehavior) {
    using Inheritable =
        ::jxx::lang::InheritableThreadLocal<::jxx::lang::String>;

    const auto local = ::jxx::NEW<Inheritable>();
    const auto value = ::jxx::NEW<::jxx::lang::String>("value");
    local->set(value);
    EXPECT_EQ(local->get(), value);
    local->remove();
    EXPECT_EQ(local->get(), nullptr);
}
