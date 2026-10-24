#include <gtest/gtest.h>
#include <type_traits>
#include "lang/jxx.lang.AutoCloseable.h"
#include "lang/jxx.lang.Object.h"
#include "util/stream/jxx.util.stream.BaseStream.h"
#include "util/stream/jxx.util.stream.IntStream.h"
#include "util/stream/jxx.util.stream.LongStream.h"
#include "util/stream/jxx.util.stream.DoubleStream.h"
TEST(StreamHierarchyTest, InterfacesDoNotInheritObject) {
 EXPECT_FALSE((std::is_base_of_v<::jxx::lang::Object, ::jxx::util::stream::IntStream>));
 EXPECT_FALSE((std::is_base_of_v<::jxx::lang::Object, ::jxx::util::stream::LongStream>));
 EXPECT_FALSE((std::is_base_of_v<::jxx::lang::Object, ::jxx::util::stream::DoubleStream>));
}
TEST(StreamHierarchyTest, PrimitiveStreamsTransitivelyInheritAutoCloseable) {
 EXPECT_TRUE((std::is_base_of_v<::jxx::lang::AutoCloseable, ::jxx::util::stream::IntStream>));
 EXPECT_TRUE((std::is_base_of_v<::jxx::lang::AutoCloseable, ::jxx::util::stream::LongStream>));
 EXPECT_TRUE((std::is_base_of_v<::jxx::lang::AutoCloseable, ::jxx::util::stream::DoubleStream>));
}
TEST(StreamHierarchyTest, BasicFactoriesReturnInterfaceInstances) {
 EXPECT_EQ(1, ::jxx::util::stream::IntStream::of(7)->count());
 EXPECT_EQ(1, ::jxx::util::stream::LongStream::of(7)->count());
 EXPECT_EQ(1, ::jxx::util::stream::DoubleStream::of(7.0)->count());
}
