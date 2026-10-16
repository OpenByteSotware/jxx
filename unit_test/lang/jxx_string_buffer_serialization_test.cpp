#include <gtest/gtest.h>

#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "jxx.initialize.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.StringBuffer.h"
#include "lang/jxx.lang.buildin_array.h"

namespace {
using Buffer = ::jxx::lang::StringBuffer;
using String = ::jxx::lang::String;
using BufferArray = ::jxx::lang::JxxArray<::jxx::Ptr<Buffer>, 1U>;
using BufferMatrix = ::jxx::lang::JxxArray<::jxx::Ptr<Buffer>, 2U>;

template <typename T>
::jxx::Ptr<T> roundTrip(const ::jxx::Ptr<T>& value) {
    auto bytes = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    auto output = ::jxx::NEW<::jxx::io::ObjectOutputStream>(bytes);
    output->writeObject(::jxx::CAST<::jxx::lang::Object>(value));
    output->flush();
    const auto serialized = bytes->toByteArray();
    auto byteInput = ::jxx::NEW<::jxx::io::ByteArrayInputStream>(serialized);
    auto input = ::jxx::NEW<::jxx::io::ObjectInputStream>(byteInput);
    auto object = input->readObject();
    return ::jxx::CAST<T>(object);
}

::jxx::Ptr<Buffer> buffer(const char* value) {
    return ::jxx::NEW<Buffer>(::jxx::NEW<String>(value));
}

TEST(StringBufferSerialization, RoundTripsContentCapacityAndMutability) {
    ::jxx::initialize();
    auto original = buffer("Controller-42");
    original->ensureCapacity(128);
    auto restored = roundTrip(original);
    ASSERT_NE(restored, nullptr);
    EXPECT_EQ("Controller-42", restored->toString()->utf8());
    EXPECT_GE(restored->capacity(), 128);
    restored->append(::jxx::NEW<String>("-restored"));
    EXPECT_EQ("Controller-42-restored", restored->toString()->utf8());
}

TEST(StringBufferSerialization, RoundTripsReferenceArrays) {
    ::jxx::initialize();
    auto array = ::jxx::NEW<BufferArray>(2);
    (*array)[0] = buffer("one");
    (*array)[1] = buffer("two");
    auto restoredArray = roundTrip(array);
    ASSERT_NE(restoredArray, nullptr);
    ASSERT_EQ(2, restoredArray->length);
    EXPECT_EQ("one", (*restoredArray)[0]->toString()->utf8());
    EXPECT_EQ("two", (*restoredArray)[1]->toString()->utf8());

    using Row = BufferMatrix::SubArray;
    auto matrix = ::jxx::NEW<BufferMatrix>(1);
    auto row = ::jxx::NEW<Row>(1);
    (*row)[0] = buffer("matrix");
    matrix->row(0) = row;
    auto restoredMatrix = roundTrip(matrix);
    const auto& restoredRow = restoredMatrix->row(0);
    ASSERT_NE(restoredRow, nullptr);
    EXPECT_EQ("matrix", (*restoredRow)[0]->toString()->utf8());
}

TEST(StringBufferSerialization, NoDataRestoresEmptyDefaultState) {
    auto value = buffer("changed");
    value->readObjectNoData();
    EXPECT_EQ(0, value->length());
    EXPECT_EQ("", value->toString()->utf8());
    EXPECT_GE(value->capacity(), 16);
}
} // namespace
