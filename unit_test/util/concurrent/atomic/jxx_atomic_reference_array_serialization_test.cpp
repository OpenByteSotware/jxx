#include <gtest/gtest.h>

#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.InvalidObjectException.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "lang/jxx.lang.String.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.AtomicReferenceArray.h"

namespace {
using Array = ::jxx::util::concurrent::atomic::AtomicReferenceArray<
    ::jxx::lang::String>;
using String = ::jxx::lang::String;

::jxx::Ptr<Array> roundTrip(const ::jxx::Ptr<Array>& value) {
    auto bytes = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    auto output = ::jxx::NEW<::jxx::io::ObjectOutputStream>(bytes);
    value->writeObject(output);
    output->flush();
    auto input = ::jxx::NEW<::jxx::io::ObjectInputStream>(
        ::jxx::NEW<::jxx::io::ByteArrayInputStream>(bytes->toByteArray()));
    auto restored = ::jxx::NEW<Array>(0);
    restored->readObject(input);
    return restored;
}

TEST(AtomicReferenceArraySerialization,
    RoundTripsNullAndSharedReferences) {
    auto original = ::jxx::NEW<Array>(3);
    auto shared = ::jxx::NEW<String>("shared");
    original->set(0, shared);
    original->set(1, nullptr);
    original->set(2, shared);

    auto restored = roundTrip(original);

    ASSERT_NE(restored, nullptr);
    ASSERT_EQ(3, restored->length());
    ASSERT_NE(restored->get(0), nullptr);
    EXPECT_EQ("shared", restored->get(0)->utf8());
    EXPECT_EQ(nullptr, restored->get(1));
    ASSERT_NE(restored->get(2), nullptr);
    EXPECT_EQ(restored->get(0).get(), restored->get(2).get());
    restored->set(1, ::jxx::NEW<String>("mutable"));
    EXPECT_EQ("mutable", restored->get(1)->utf8());
}

TEST(AtomicReferenceArraySerialization, RoundTripsEmptyArray) {
    auto restored = roundTrip(::jxx::NEW<Array>(0));
    ASSERT_NE(restored, nullptr);
    EXPECT_EQ(0, restored->length());
}

TEST(AtomicReferenceArraySerialization, NoDataRestoresEmptyArray) {
    auto value = ::jxx::NEW<Array>(2);
    value->set(0, ::jxx::NEW<String>("changed"));
    value->readObjectNoData();
    EXPECT_EQ(0, value->length());
}

TEST(AtomicReferenceArraySerialization, RejectsInvalidLength) {
    auto bytes = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    auto output = ::jxx::NEW<::jxx::io::ObjectOutputStream>(bytes);
    output->writeInt(-1);
    output->flush();
    auto input = ::jxx::NEW<::jxx::io::ObjectInputStream>(
        ::jxx::NEW<::jxx::io::ByteArrayInputStream>(bytes->toByteArray()));
    auto value = ::jxx::NEW<Array>(0);
    EXPECT_THROW(value->readObject(input), ::jxx::io::InvalidObjectException);
}

TEST(AtomicReferenceArraySerialization, RejectsNullStreams) {
    auto value = ::jxx::NEW<Array>(0);
    EXPECT_THROW(value->writeObject(nullptr), ::jxx::lang::NullPointerException);
    EXPECT_THROW(value->readObject(nullptr), ::jxx::lang::NullPointerException);
}

} // namespace
