#include <gtest/gtest.h>

#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.AtomicBoolean.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.AtomicInteger.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.AtomicLong.h"
#include "jxx.initialize.h"
namespace {

template <typename T>
::jxx::Ptr<T> roundTrip(const ::jxx::Ptr<T>& value) {
    auto bytes = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    auto output = ::jxx::NEW<::jxx::io::ObjectOutputStream>(bytes);
    output->writeObject(::jxx::CAST<::jxx::lang::Object>(value));
    output->flush();

    auto inputBytes = ::jxx::NEW<::jxx::io::ByteArrayInputStream>(bytes->toByteArray());
    auto input = ::jxx::NEW<::jxx::io::ObjectInputStream>(inputBytes);
    return ::jxx::CAST<T>(input->readObject());
}

class AtomicScalarSerializationTest : public ::testing::Test {
protected:
    static void SetUpTestSuite() {
        ::jxx::initialize();
    }
};

TEST_F(AtomicScalarSerializationTest, AtomicBooleanRoundTrip) {
    using Type = ::jxx::util::concurrent::atomic::AtomicBoolean;
    auto restored = roundTrip(::jxx::NEW<Type>(true));
    ASSERT_NE(restored, nullptr);
    EXPECT_TRUE(restored->get());
}

TEST_F(AtomicScalarSerializationTest, AtomicIntegerRoundTrip) {
    using Type = ::jxx::util::concurrent::atomic::AtomicInteger;
    auto restored = roundTrip(::jxx::NEW<Type>(123456));
    ASSERT_NE(restored, nullptr);
    EXPECT_EQ(restored->get(), 123456);
}

TEST_F(AtomicScalarSerializationTest, AtomicLongRoundTrip) {
    using Type = ::jxx::util::concurrent::atomic::AtomicLong;
    auto restored = roundTrip(::jxx::NEW<Type>(1234567890123LL));
    ASSERT_NE(restored, nullptr);
    EXPECT_EQ(restored->get(), 1234567890123LL);
}

TEST_F(AtomicScalarSerializationTest, ClassMetadataIncludesSerializableI) {
    using ::jxx::io::SerializableI;
    using ::jxx::util::concurrent::atomic::AtomicBoolean;
    using ::jxx::util::concurrent::atomic::AtomicInteger;
    using ::jxx::util::concurrent::atomic::AtomicLong;

    EXPECT_NE(::jxx::CAST<SerializableI>(::jxx::NEW<AtomicBoolean>()), nullptr);
    EXPECT_NE(::jxx::CAST<SerializableI>(::jxx::NEW<AtomicInteger>()), nullptr);
    EXPECT_NE(::jxx::CAST<SerializableI>(::jxx::NEW<AtomicLong>()), nullptr);
}

} // namespace
