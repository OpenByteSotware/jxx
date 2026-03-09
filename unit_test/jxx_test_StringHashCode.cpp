#include <gtest/gtest.h>
#include "jxx.lang.String.h"

using namespace jxx::lang;

TEST(StringHashCode, EmptyAndAscii) {
    String empty("");
    EXPECT_EQ(0, empty.hashCode());

    String abc("abc");
    // Java hashCode for "abc" = ((('a')*31 + 'b')*31 + 'c') = 96354
    EXPECT_EQ(96354, abc.hashCode());
}

TEST(StringHashCode, KnownCollisionPairs) {
    // Famous Java collisions: "Aa" and "BB" both hash to 2112
    String s1("Aa");
    String s2("BB");
    EXPECT_EQ(2112, s1.hashCode());
    EXPECT_EQ(2112, s2.hashCode());
}
