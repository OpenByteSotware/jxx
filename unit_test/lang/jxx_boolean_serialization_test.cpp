#include <gtest/gtest.h>

#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "jxx.initialize.h"
#include "lang/jxx.lang.Boolean.h"
#include "lang/jxx.lang.JxxArray.h"

namespace {

using Boolean = ::jxx::lang::Boolean;
using BooleanArray = ::jxx::lang::JxxArray<::jxx::Ptr<Boolean>, 1U>;
using BooleanMatrix = ::jxx::lang::JxxArray<::jxx::Ptr<Boolean>, 2U>;

template <typename T>
::jxx::Ptr<T> roundTrip(const ::jxx::Ptr<T>& value) {
    auto bytes = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    {
        auto output = ::jxx::NEW<::jxx::io::ObjectOutputStream>(bytes);
        output->writeObject(::jxx::CAST<::jxx::lang::Object>(value));
        output->flush();
    }
    auto inputBytes = ::jxx::NEW<::jxx::io::ByteArrayInputStream>(
        bytes->toByteArray());
    auto input = ::jxx::NEW<::jxx::io::ObjectInputStream>(inputBytes);
    return ::jxx::CAST<T>(input->readObject());
}

TEST(BooleanSerialization, RoundTripsTrueAndFalse) {
    ::jxx::initialize();

    auto restoredTrue = roundTrip(::jxx::NEW<Boolean>(true));
    auto restoredFalse = roundTrip(::jxx::NEW<Boolean>(false));

    ASSERT_NE(restoredTrue, nullptr);
    ASSERT_NE(restoredFalse, nullptr);
    EXPECT_TRUE(restoredTrue->booleanValue());
    EXPECT_FALSE(restoredFalse->booleanValue());
}

TEST(BooleanSerialization, RoundTripsTypedArrays) {
    ::jxx::initialize();
    auto array = ::jxx::NEW<BooleanArray>(2);
    (*array)[0] = ::jxx::NEW<Boolean>(true);
    (*array)[1] = ::jxx::NEW<Boolean>(false);

    auto restoredArray = roundTrip(array);
    ASSERT_NE(restoredArray, nullptr);
    ASSERT_EQ(2, restoredArray->length);
    EXPECT_TRUE((*restoredArray)[0]->booleanValue());
    EXPECT_FALSE((*restoredArray)[1]->booleanValue());

    using Row = BooleanMatrix::SubArray;
    auto matrix = ::jxx::NEW<BooleanMatrix>(1);
    auto row = ::jxx::NEW<Row>(1);
    (*row)[0] = ::jxx::NEW<Boolean>(true);
    matrix->row(0) = row;

    auto restoredMatrix = roundTrip(matrix);
    ASSERT_NE(restoredMatrix, nullptr);
    const auto& restoredRow = restoredMatrix->row(0);
    ASSERT_NE(restoredRow, nullptr);
    EXPECT_TRUE((*restoredRow)[0]->booleanValue());
}

TEST(BooleanSerialization, NoDataRestoresFalse) {
    auto value = ::jxx::NEW<Boolean>(true);
    value->readObjectNoData();
    EXPECT_FALSE(value->booleanValue());
}

} // namespace
