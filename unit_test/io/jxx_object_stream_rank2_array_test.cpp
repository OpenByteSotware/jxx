#include <gtest/gtest.h>
#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "lang/jxx.lang.Cast.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"
#include "jxx.initialize.h"

TEST(JxxObjectStreamRank2Array, RoundTripsJaggedPrimitiveArrayWithNullAndSharedRows) {
    jxx::initialize();
    auto value = jxx::NEW<jxx::lang::IntArray2DType>(3);
    auto row = jxx::NEW<jxx::lang::IntArrayType>(2);
    (*row)[0] = 17; (*row)[1] = -4;
    (*value)[0] = row; (*value)[1] = nullptr; (*value)[2] = row;
    auto bytes = jxx::NEW<jxx::io::ByteArrayOutputStream>();
    auto output = jxx::NEW<jxx::io::ObjectOutputStream>(bytes);
    output->writeObject(value); output->flush();
    auto input = jxx::NEW<jxx::io::ObjectInputStream>(jxx::NEW<jxx::io::ByteArrayInputStream>(bytes->toByteArray()));
    auto restored = jxx::CAST<jxx::lang::IntArray2DType>(input->readObject());
    ASSERT_EQ(3, restored->length);
    EXPECT_EQ(nullptr, static_cast<jxx::lang::IntArray2DType::RowPointer>((*restored)[1]));
    auto first = static_cast<jxx::lang::IntArray2DType::RowPointer>((*restored)[0]);
    auto third = static_cast<jxx::lang::IntArray2DType::RowPointer>((*restored)[2]);
    ASSERT_NE(nullptr, first); EXPECT_EQ(first.get(), third.get());
    EXPECT_EQ(17, (*first)[0]); EXPECT_EQ(-4, (*first)[1]);
}

TEST(JxxObjectStreamRank2Array, RoundTripsObjectRowsAndSharedElements) {
    auto value = jxx::NEW<jxx::lang::ObjectArray2DType>(1);
    auto row = jxx::NEW<jxx::lang::ObjectArrayType>(2);
    auto text = jxx::NEW<jxx::lang::String>("shared");
    (*row)[0] = text; (*row)[1] = text; (*value)[0] = row;
    auto bytes = jxx::NEW<jxx::io::ByteArrayOutputStream>();
    auto output = jxx::NEW<jxx::io::ObjectOutputStream>(bytes);
    output->writeObject(value); output->flush();
    auto input = jxx::NEW<jxx::io::ObjectInputStream>(jxx::NEW<jxx::io::ByteArrayInputStream>(bytes->toByteArray()));
    auto restored = jxx::CAST<jxx::lang::ObjectArray2DType>(input->readObject());
    auto restoredRow = static_cast<jxx::lang::ObjectArray2DType::RowPointer>((*restored)[0]);
    ASSERT_NE(nullptr, restoredRow); EXPECT_EQ((*restoredRow)[0].get(), (*restoredRow)[1].get());
}
