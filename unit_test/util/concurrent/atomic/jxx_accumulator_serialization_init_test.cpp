#include <gtest/gtest.h>
#include <type_traits>

#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "io/jxx.io.SerializationClassRegistry.h"
#include "../../../jxx_accumulator_serialization_test_support.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.ClassInitialization.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.DoubleAccumulator.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.LongAccumulator.h"

namespace
{
void initializeSerialization()
{
    ::jxx::util::concurrent::atomic::initializeClasses();
    ::jxx::io::serialization::registerSerializableClass<
        ::jxx::unit_test::atomic::SerializableDoubleSumOperator>(1);
    ::jxx::io::serialization::registerSerializableClass<
        ::jxx::unit_test::atomic::SerializableLongSumOperator>(1);
}

template <typename T>
::jxx::Ptr<T> roundTrip(const ::jxx::Ptr<T>& value)
{
    auto bytes = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    {
        auto output = ::jxx::NEW<::jxx::io::ObjectOutputStream>(bytes);
        output->writeObject(::jxx::CAST<::jxx::lang::Object>(value));
        output->flush();
    }
    auto inputBytes = ::jxx::NEW<::jxx::io::ByteArrayInputStream>(bytes->toByteArray());
    auto input = ::jxx::NEW<::jxx::io::ObjectInputStream>(inputBytes);
    return ::jxx::CAST<T>(input->readObject());
}
}

TEST(AccumulatorSerializationInitialization, RegistersPrivateFactories)
{
    initializeSerialization();
    const auto doubleMetadata = ::jxx::io::serialization::SerializationClassRegistry::resolve(
        ::jxx::util::concurrent::atomic::DoubleAccumulator::Class()->getName()->utf8());
    const auto longMetadata = ::jxx::io::serialization::SerializationClassRegistry::resolve(
        ::jxx::util::concurrent::atomic::LongAccumulator::Class()->getName()->utf8());
    ASSERT_TRUE(static_cast<bool>(doubleMetadata.deserializationFactory));
    ASSERT_TRUE(static_cast<bool>(longMetadata.deserializationFactory));
    EXPECT_NE(nullptr, doubleMetadata.deserializationFactory());
    EXPECT_NE(nullptr, longMetadata.deserializationFactory());
}

TEST(AccumulatorSerializationInitialization, PublicDefaultConstructionRemainsUnavailable)
{
    EXPECT_FALSE(std::is_default_constructible_v<
        ::jxx::util::concurrent::atomic::DoubleAccumulator>);
    EXPECT_FALSE(std::is_default_constructible_v<
        ::jxx::util::concurrent::atomic::LongAccumulator>);
}

TEST(AccumulatorSerializationRoundTrip, RestoresDoubleFunctionIdentityAndValue)
{
    initializeSerialization();
    auto function = ::jxx::NEW<::jxx::unit_test::atomic::SerializableDoubleSumOperator>();
    auto value = ::jxx::NEW<::jxx::util::concurrent::atomic::DoubleAccumulator>(function, 2.5);
    value->accumulate(4.0);
    auto copy = roundTrip(value);
    ASSERT_NE(nullptr, copy);
    EXPECT_DOUBLE_EQ(6.5, copy->get());
    copy->accumulate(1.5);
    EXPECT_DOUBLE_EQ(8.0, copy->get());
    copy->reset();
    EXPECT_DOUBLE_EQ(2.5, copy->get());
}

TEST(AccumulatorSerializationRoundTrip, RestoresLongFunctionIdentityAndValue)
{
    initializeSerialization();
    auto function = ::jxx::NEW<::jxx::unit_test::atomic::SerializableLongSumOperator>();
    auto value = ::jxx::NEW<::jxx::util::concurrent::atomic::LongAccumulator>(function, 3);
    value->accumulate(7);
    auto copy = roundTrip(value);
    ASSERT_NE(nullptr, copy);
    EXPECT_EQ(10, copy->get());
    copy->accumulate(5);
    EXPECT_EQ(15, copy->get());
    copy->reset();
    EXPECT_EQ(3, copy->get());
}
