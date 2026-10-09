#include <gtest/gtest.h>
#include <type_traits>

#include "util/jxx.util.EventObject.h"

TEST(JxxEventObjectJava8HierarchyTest, IsSerializableObjectClass) {
    static_assert(std::is_base_of_v<::jxx::lang::Object,
                                    ::jxx::util::EventObject>);
    static_assert(std::is_base_of_v<::jxx::io::SerializableI,
                                    ::jxx::util::EventObject>);
    EXPECT_TRUE(::jxx::io::SerializableI::Class()->isAssignableFrom(
        ::jxx::util::EventObject::Class()));
}
