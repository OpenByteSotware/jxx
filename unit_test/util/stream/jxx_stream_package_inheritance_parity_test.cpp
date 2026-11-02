#include <gtest/gtest.h>
#include <type_traits>

#include "lang/jxx.lang.AutoCloseable.h"
#include "lang/jxx.lang.Double.h"
#include "lang/jxx.lang.Integer.h"
#include "lang/jxx.lang.Long.h"
#include "util/stream/jxx.util.stream.BaseStream.h"
#include "util/stream/jxx.util.stream.ClassInitialization.h"
#include "util/stream/jxx.util.stream.DoubleStream.h"
#include "util/stream/jxx.util.stream.IntStream.h"
#include "util/stream/jxx.util.stream.LongStream.h"
#include "jxx.initialize.h"

namespace stream = ::jxx::util::stream;

TEST(StreamPackageInheritanceParity, BaseStreamExtendsAutoCloseable) {
    using IntBase = stream::BaseStream<::jxx::lang::Integer, stream::IntStream>;
    static_assert(std::is_base_of_v<::jxx::lang::AutoCloseable, IntBase>);
    SUCCEED();
}

TEST(StreamPackageInheritanceParity, PrimitiveStreamsExtendTheirBaseStreamSpecializations) {
    static_assert(std::is_base_of_v<
        stream::BaseStream<::jxx::lang::Integer, stream::IntStream>,
        stream::IntStream>);
    static_assert(std::is_base_of_v<
        stream::BaseStream<::jxx::lang::Long, stream::LongStream>,
        stream::LongStream>);
    static_assert(std::is_base_of_v<
        stream::BaseStream<::jxx::lang::Double, stream::DoubleStream>,
        stream::DoubleStream>);
    SUCCEED();
}

TEST(StreamPackageInitializationParity, RegistersPublicInterfacesAndParentMetadata) {
    jxx::initialize();

    EXPECT_NE(nullptr, stream::IntStream::Class());
    EXPECT_NE(nullptr, stream::LongStream::Class());
    EXPECT_NE(nullptr, stream::DoubleStream::Class());
}
