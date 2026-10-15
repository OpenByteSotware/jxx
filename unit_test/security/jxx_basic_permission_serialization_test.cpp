#include <gtest/gtest.h>

#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.InvalidObjectException.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "jxx.initialize.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"
#include "security/jxx.security.BasicPermission.h"

namespace {

using Permission = ::jxx::security::BasicPermission;
using String = ::jxx::lang::String;
using PermissionArray =
    ::jxx::lang::JxxArray<::jxx::Ptr<Permission>, 1U>;
using PermissionMatrix =
    ::jxx::lang::JxxArray<::jxx::Ptr<Permission>, 2U>;

template <typename T>
::jxx::Ptr<T> roundTrip(const ::jxx::Ptr<T>& value) {
    auto bytes = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    {
        auto output = ::jxx::NEW<::jxx::io::ObjectOutputStream>(bytes);
        output->writeObject(::jxx::CAST<::jxx::lang::Object>(value));
        output->flush();
    }
    auto byteInput =
        ::jxx::NEW<::jxx::io::ByteArrayInputStream>(bytes->toByteArray());
    auto input = ::jxx::NEW<::jxx::io::ObjectInputStream>(byteInput);
    return ::jxx::CAST<T>(input->readObject());
}

TEST(BasicPermissionSerialization, RoundTripsExactPermission) {
    ::jxx::initialize();
    auto original = ::jxx::NEW<Permission>(
        ::jxx::NEW<String>("service.read"));

    auto restored = roundTrip(original);

    ASSERT_NE(restored, nullptr);
    EXPECT_TRUE(original->equals(restored));
    EXPECT_EQ("service.read", restored->getName()->utf8());
    EXPECT_EQ("", restored->getActions()->utf8());
}

TEST(BasicPermissionSerialization, RebuildsWildcardDerivedState) {
    ::jxx::initialize();
    auto original = ::jxx::NEW<Permission>(
        ::jxx::NEW<String>("service.*"));

    auto restored = roundTrip(original);
    auto child = ::jxx::NEW<Permission>(
        ::jxx::NEW<String>("service.read"));
    auto other = ::jxx::NEW<Permission>(
        ::jxx::NEW<String>("other.read"));

    ASSERT_NE(restored, nullptr);
    EXPECT_TRUE(restored->implies(child));
    EXPECT_FALSE(restored->implies(other));
}

TEST(BasicPermissionSerialization, RoundTripsTypedArrays) {
    ::jxx::initialize();
    auto array = ::jxx::NEW<PermissionArray>(2);
    (*array)[0] = ::jxx::NEW<Permission>(::jxx::NEW<String>("one"));
    (*array)[1] = ::jxx::NEW<Permission>(::jxx::NEW<String>("two.*"));

    auto restoredArray = roundTrip(array);
    ASSERT_NE(restoredArray, nullptr);
    ASSERT_EQ(2, restoredArray->length);
    EXPECT_EQ("one", (*restoredArray)[0]->getName()->utf8());
    EXPECT_EQ("two.*", (*restoredArray)[1]->getName()->utf8());

    using Row = PermissionMatrix::SubArray;
    auto matrix = ::jxx::NEW<PermissionMatrix>(1);
    auto row = ::jxx::NEW<Row>(1);
    (*row)[0] = ::jxx::NEW<Permission>(::jxx::NEW<String>("matrix"));
    matrix->row(0) = row;

    auto restoredMatrix = roundTrip(matrix);
    ASSERT_NE(restoredMatrix, nullptr);
    const auto& restoredRow = restoredMatrix->row(0);
    ASSERT_NE(restoredRow, nullptr);
    EXPECT_EQ("matrix", (*restoredRow)[0]->getName()->utf8());
}

TEST(BasicPermissionSerialization, NoDataIsRejected) {
    auto permission = ::jxx::NEW<Permission>(
        ::jxx::NEW<String>("service.read"));
    EXPECT_THROW(
        permission->readObjectNoData(),
        ::jxx::io::InvalidObjectException);
}

} // namespace
