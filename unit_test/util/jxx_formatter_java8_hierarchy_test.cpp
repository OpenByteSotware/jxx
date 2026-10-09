#include <gtest/gtest.h>
#include <type_traits>

#include "util/jxx.util.Formatter.h"

TEST(JxxFormatterJava8HierarchyTest, IsFinalCloseableAndFlushable) {
    static_assert(std::is_final_v<::jxx::util::Formatter>);
    static_assert(std::is_base_of_v<::jxx::io::Closeable,
                                    ::jxx::util::Formatter>);
    static_assert(std::is_base_of_v<::jxx::io::Flushable,
                                    ::jxx::util::Formatter>);
    EXPECT_TRUE(::jxx::io::Closeable::Class()->isAssignableFrom(
        ::jxx::util::Formatter::Class()));
    EXPECT_TRUE(::jxx::io::Flushable::Class()->isAssignableFrom(
        ::jxx::util::Formatter::Class()));
}

TEST(JxxFormatterJava8HierarchyTest, CloseAndFlushRemainCallable) {
    const auto formatter = ::jxx::NEW<::jxx::util::Formatter>();
    formatter->flush();
    formatter->close();
    EXPECT_TRUE(formatter->closed());
}
