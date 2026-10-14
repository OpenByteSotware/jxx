#include <gtest/gtest.h>

#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "lang/jxx.lang.Cast.h"
#include "lang/jxx.lang.String.h"

namespace {

TEST(JxxObjectStreamFormat, RoundTripsNullAndUtf16String) {
    auto bytes = jxx::NEW<jxx::io::ByteArrayOutputStream>();
    {
        auto output = jxx::NEW<jxx::io::ObjectOutputStream>(bytes);
        output->writeObject(nullptr);
        output->writeObject(jxx::NEW<jxx::lang::String>(std::u16string(u"A\u03A9\U0001F600")));
        output->flush();
    }

    auto inputBytes = jxx::NEW<jxx::io::ByteArrayInputStream>(bytes->toByteArray());
    auto input = jxx::NEW<jxx::io::ObjectInputStream>(inputBytes);
    EXPECT_EQ(nullptr, input->readObject());
    const auto value = jxx::CAST<jxx::lang::String>(input->readObject());
    ASSERT_NE(nullptr, value);
    EXPECT_EQ(std::u16string(u"A\u03A9\U0001F600"), value->utf16());
}

TEST(JxxObjectStreamFormat, PreservesRepeatedStringReference) {
    auto value = jxx::NEW<jxx::lang::String>("shared");
    auto bytes = jxx::NEW<jxx::io::ByteArrayOutputStream>();
    auto output = jxx::NEW<jxx::io::ObjectOutputStream>(bytes);
    output->writeObject(value);
    output->writeObject(value);
    output->flush();

    auto inputBytes = jxx::NEW<jxx::io::ByteArrayInputStream>(bytes->toByteArray());
    auto input = jxx::NEW<jxx::io::ObjectInputStream>(inputBytes);
    const auto first = input->readObject();
    const auto second = input->readObject();
    EXPECT_EQ(first.get(), second.get());
}

} // namespace

TEST(JxxObjectStreamFormat, RoundTripsPrimitiveArrays) {
    auto integers = jxx::NEW<jxx::lang::IntArrayType>(3);
    (*integers)[0] = -7; (*integers)[1] = 0; (*integers)[2] = 42;
    auto doubles = jxx::NEW<jxx::lang::DoubleArrayType>(2);
    (*doubles)[0] = -0.0; (*doubles)[1] = 3.5;
    auto bytes = jxx::NEW<jxx::io::ByteArrayOutputStream>();
    auto output = jxx::NEW<jxx::io::ObjectOutputStream>(bytes);
    output->writeObject(integers); output->writeObject(doubles); output->flush();
    auto input = jxx::NEW<jxx::io::ObjectInputStream>(jxx::NEW<jxx::io::ByteArrayInputStream>(bytes->toByteArray()));
    auto ri = jxx::CAST<jxx::lang::IntArrayType>(input->readObject());
    auto rd = jxx::CAST<jxx::lang::DoubleArrayType>(input->readObject());
    ASSERT_EQ(3, ri->length); EXPECT_EQ(-7, (*ri)[0]); EXPECT_EQ(42, (*ri)[2]);
    ASSERT_EQ(2, rd->length); EXPECT_DOUBLE_EQ(-0.0, (*rd)[0]); EXPECT_DOUBLE_EQ(3.5, (*rd)[1]);
}

TEST(JxxObjectStreamFormat, PreservesObjectArrayNullSharingAndCycle) {
    auto array = jxx::NEW<jxx::lang::ObjectArrayType>(4);
    auto shared = jxx::NEW<jxx::lang::String>("shared");
    (*array)[0] = shared; (*array)[1] = nullptr; (*array)[2] = shared; (*array)[3] = array;
    auto bytes = jxx::NEW<jxx::io::ByteArrayOutputStream>();
    auto output = jxx::NEW<jxx::io::ObjectOutputStream>(bytes);
    output->writeObject(array); output->flush();
    auto input = jxx::NEW<jxx::io::ObjectInputStream>(jxx::NEW<jxx::io::ByteArrayInputStream>(bytes->toByteArray()));
    auto restored = jxx::CAST<jxx::lang::ObjectArrayType>(input->readObject());
    ASSERT_EQ(4, restored->length);
    EXPECT_EQ((*restored)[0].get(), (*restored)[2].get());
    EXPECT_EQ(nullptr, (*restored)[1]);
    EXPECT_EQ(restored.get(), (*restored)[3].get());
}
