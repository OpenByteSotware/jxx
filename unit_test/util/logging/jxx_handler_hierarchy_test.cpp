#include <gtest/gtest.h>
#include <type_traits>
#include "util/logging/jxx.util.logging.Handler.h"

TEST(JxxHandlerHierarchyTest, IsAbstractObjectClassWithClassBaseMetadata) {
    static_assert(std::is_abstract_v<::jxx::util::logging::Handler>);
    static_assert(std::is_base_of_v<::jxx::lang::Object,
                                    ::jxx::util::logging::Handler>);
    EXPECT_NE(::jxx::util::logging::Handler::Class(), nullptr);
}
