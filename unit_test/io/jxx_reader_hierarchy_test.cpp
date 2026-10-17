#include <gtest/gtest.h>
#include <type_traits>
#include "io/jxx.io.Closeable.h"
#include "io/jxx.io.Reader.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.Readable.h"

TEST(ReaderHierarchyParity, MatchesJava8Interfaces) {
    EXPECT_TRUE((std::is_base_of_v<::jxx::lang::Object, ::jxx::io::Reader>));
    EXPECT_TRUE((std::is_base_of_v<::jxx::lang::Readable, ::jxx::io::Reader>));
    EXPECT_TRUE((std::is_base_of_v<::jxx::io::Closeable, ::jxx::io::Reader>));
    EXPECT_TRUE(std::is_abstract_v<::jxx::io::Reader>);
}
