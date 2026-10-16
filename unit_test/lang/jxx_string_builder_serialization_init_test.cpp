#include <gtest/gtest.h>

#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "jxx.initialize.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.StringBuilder.h"
#include "lang/jxx.lang.buildin_array.h"

namespace {

using Builder = ::jxx::lang::StringBuilder;
using String = ::jxx::lang::String;
using BuilderArray = ::jxx::lang::JxxArray<::jxx::Ptr<Builder>, 1U>;
using BuilderMatrix = ::jxx::lang::JxxArray<::jxx::Ptr<Builder>, 2U>;

template <typename T>
::jxx::Ptr<T> roundTrip(
    const ::jxx::Ptr<T>& value)
{
    auto bytes =
        ::jxx::NEW<
        ::jxx::io::ByteArrayOutputStream>();

    auto output =
        ::jxx::NEW<
        ::jxx::io::ObjectOutputStream>(
            bytes);

    output->writeObject(
        ::jxx::CAST<::jxx::lang::Object>(
            value));

    output->flush();

    const auto serialized =
        bytes->toByteArray();

    auto byteInput =
        ::jxx::NEW<
        ::jxx::io::ByteArrayInputStream>(
            serialized);

    auto input =
        ::jxx::NEW<
        ::jxx::io::ObjectInputStream>(
            byteInput);

    auto object =
        input->readObject();

    return ::jxx::CAST<T>(object);
}

::jxx::Ptr<Builder> builder(const char* value)
{
    return ::jxx::NEW<Builder>(::jxx::NEW<String>(value));
}

TEST(
    StringBuilderSerializationInitialization,
    RoundTripsUtf16ContentSimple)
{
    ::jxx::initialize();

    auto original = builder("Controller-42");
    ASSERT_NE(original, nullptr);
    ASSERT_EQ(13, original->length());

    auto bytes =
        ::jxx::NEW<
        ::jxx::io::ByteArrayOutputStream>();

    ASSERT_NE(bytes, nullptr);

    auto output =
        ::jxx::NEW<
        ::jxx::io::ObjectOutputStream>(
            bytes);

    ASSERT_NE(output, nullptr);

    output->writeObject(
        ::jxx::CAST<::jxx::lang::Object>(
            original));

    output->flush();

    const auto serialized =
        bytes->toByteArray();

    ASSERT_NE(serialized, nullptr);
    ASSERT_GT(serialized->length, 0);

    auto byteInput =
        ::jxx::NEW<
        ::jxx::io::ByteArrayInputStream>(
            serialized);

    ASSERT_NE(byteInput, nullptr);

    auto input =
        ::jxx::NEW<
        ::jxx::io::ObjectInputStream>(
            byteInput);

    ASSERT_NE(input, nullptr);

    auto object =
        input->readObject();

    ASSERT_NE(object, nullptr);

    auto restored =
        ::jxx::CAST<Builder>(object);

    ASSERT_NE(restored, nullptr);
    EXPECT_EQ(
        original->length(),
        restored->length());

    EXPECT_EQ(
        original->toString()->utf8(),
        restored->toString()->utf8());

    auto suffix =
        ::jxx::NEW<String>("-restored");

    ASSERT_NE(suffix, nullptr);

    auto appendResult =
        restored->append(suffix);

    ASSERT_NE(appendResult, nullptr);

    EXPECT_EQ(
        "Controller-42-restored",
        restored->toString()->utf8());
}

TEST(StringBuilderSerializationInitialization, RoundTripsUtf16Content) {
    ::jxx::initialize();
    auto original = builder("Controller-42");

    auto restored = roundTrip(original);

    ASSERT_NE(restored, nullptr);
    EXPECT_EQ(original->length(), restored->length());
    EXPECT_EQ(original->toString()->utf8(), restored->toString()->utf8());
    restored->append(::jxx::NEW<String>("-restored"));
    EXPECT_EQ("Controller-42-restored", restored->toString()->utf8());
}

TEST(StringBuilderSerializationInitialization, RoundTripsRankOneArray) {
    ::jxx::initialize();
    auto original = ::jxx::NEW<BuilderArray>(2);
    (*original)[0] = builder("one");
    (*original)[1] = builder("two");

    auto restored = roundTrip(original);

    ASSERT_NE(restored, nullptr);
    ASSERT_EQ(2, restored->length);
    EXPECT_EQ("one", (*restored)[0]->toString()->utf8());
    EXPECT_EQ("two", (*restored)[1]->toString()->utf8());
}

TEST(StringBuilderSerializationInitialization, RoundTripsRankTwoArray) {
    ::jxx::initialize();
    using Row = BuilderMatrix::SubArray;
    auto original = ::jxx::NEW<BuilderMatrix>(1);
    auto row = ::jxx::NEW<Row>(2);
    (*row)[0] = builder("left");
    (*row)[1] = builder("right");
    original->row(0) = row;

    auto restored = roundTrip(original);

    ASSERT_NE(restored, nullptr);
    const auto& restoredRow = restored->row(0);
    ASSERT_NE(restoredRow, nullptr);
    EXPECT_EQ("left", (*restoredRow)[0]->toString()->utf8());
    EXPECT_EQ("right", (*restoredRow)[1]->toString()->utf8());
}

TEST(StringBuilderSerializationInitialization, NoDataRestoresEmptyState) {
    auto value = builder("changed");
    value->readObjectNoData();
    EXPECT_EQ(0, value->length());
    EXPECT_EQ("", value->toString()->utf8());
}

} // namespace
