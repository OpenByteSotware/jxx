#include <gtest/gtest.h>

#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "jxx.initialize.h"
#include "lang/jxx.lang.Character.h"
#include "lang/jxx.lang.JxxArray.h"

namespace {

using Character = ::jxx::lang::Character;
using CharacterArray =
    ::jxx::lang::JxxArray<::jxx::Ptr<Character>, 1U>;
using CharacterMatrix =
    ::jxx::lang::JxxArray<::jxx::Ptr<Character>, 2U>;

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

TEST(CharacterSerialization, RoundTripsBmpAndSurrogateCodeUnits) {
    ::jxx::initialize();

    for (const auto value : {
             static_cast<::jxx::lang::jchar>('A'),
             static_cast<::jxx::lang::jchar>(0x03A9),
             static_cast<::jxx::lang::jchar>(0xD83D)}) {
        auto restored = roundTrip(::jxx::NEW<Character>(value));
        ASSERT_NE(restored, nullptr);
        EXPECT_EQ(value, restored->charValue());
    }
}

TEST(CharacterSerialization, RoundTripsTypedArrays) {
    ::jxx::initialize();
    auto array = ::jxx::NEW<CharacterArray>(2);
    (*array)[0] = ::jxx::NEW<Character>(static_cast<::jxx::lang::jchar>('x'));
    (*array)[1] = ::jxx::NEW<Character>(static_cast<::jxx::lang::jchar>('y'));

    auto restoredArray = roundTrip(array);
    ASSERT_NE(restoredArray, nullptr);
    ASSERT_EQ(2, restoredArray->length);
    EXPECT_EQ(static_cast<::jxx::lang::jchar>('x'),
        (*restoredArray)[0]->charValue());
    EXPECT_EQ(static_cast<::jxx::lang::jchar>('y'),
        (*restoredArray)[1]->charValue());

    using Row = CharacterMatrix::SubArray;
    auto matrix = ::jxx::NEW<CharacterMatrix>(1);
    auto row = ::jxx::NEW<Row>(1);
    (*row)[0] = ::jxx::NEW<Character>(static_cast<::jxx::lang::jchar>('z'));
    matrix->row(0) = row;

    auto restoredMatrix = roundTrip(matrix);
    ASSERT_NE(restoredMatrix, nullptr);
    const auto& restoredRow = restoredMatrix->row(0);
    ASSERT_NE(restoredRow, nullptr);
    EXPECT_EQ(static_cast<::jxx::lang::jchar>('z'),
        (*restoredRow)[0]->charValue());
}

TEST(CharacterSerialization, NoDataRestoresNullCodeUnit) {
    auto value = ::jxx::NEW<Character>(static_cast<::jxx::lang::jchar>('q'));
    value->readObjectNoData();
    EXPECT_EQ(static_cast<::jxx::lang::jchar>(0), value->charValue());
}

} // namespace
