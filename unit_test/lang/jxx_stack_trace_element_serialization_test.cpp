#include <gtest/gtest.h>

#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.InvalidObjectException.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "jxx.initialize.h"
#include "lang/jxx.lang.StackTraceElement.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"

namespace {

using Element = ::jxx::lang::StackTraceElement;
using String = ::jxx::lang::String;
using ElementArray = ::jxx::lang::JxxArray<::jxx::Ptr<Element>, 1U>;
using ElementMatrix = ::jxx::lang::JxxArray<::jxx::Ptr<Element>, 2U>;

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

::jxx::Ptr<Element> element(
    const char* type,
    const char* method,
    const char* file,
    ::jxx::lang::jint line) {
    return ::jxx::NEW<Element>(
        ::jxx::NEW<String>(type),
        ::jxx::NEW<String>(method),
        file == nullptr ? nullptr : ::jxx::NEW<String>(file),
        line);
}

TEST(StackTraceElementSerialization, RoundTripsAllState) {
    ::jxx::initialize();
    auto original = element("example.Controller", "run", "Controller.cpp", 42);

    auto restored = roundTrip(original);

    ASSERT_NE(restored, nullptr);
    EXPECT_TRUE(original->equals(restored));
    EXPECT_EQ("example.Controller", restored->getClassName()->utf8());
    EXPECT_EQ("run", restored->getMethodName()->utf8());
    ASSERT_NE(restored->getFileName(), nullptr);
    EXPECT_EQ("Controller.cpp", restored->getFileName()->utf8());
    EXPECT_EQ(42, restored->getLineNumber());
}

TEST(StackTraceElementSerialization, PreservesNullFileAndNativeMarker) {
    ::jxx::initialize();
    auto noFile = roundTrip(element("example.Controller", "run", nullptr, -1));
    ASSERT_NE(noFile, nullptr);
    EXPECT_EQ(nullptr, noFile->getFileName());
    EXPECT_EQ(-1, noFile->getLineNumber());

    auto nativeMethod = roundTrip(
        element("example.Native", "invoke", "Native.cpp", -2));
    ASSERT_NE(nativeMethod, nullptr);
    EXPECT_TRUE(nativeMethod->isNativeMethod());
}

TEST(StackTraceElementSerialization, RoundTripsTypedArrays) {
    ::jxx::initialize();
    auto array = ::jxx::NEW<ElementArray>(2);
    (*array)[0] = element("A", "one", "A.cpp", 1);
    (*array)[1] = element("B", "two", "B.cpp", 2);

    auto restoredArray = roundTrip(array);
    ASSERT_NE(restoredArray, nullptr);
    ASSERT_EQ(2, restoredArray->length);
    EXPECT_EQ("A", (*restoredArray)[0]->getClassName()->utf8());
    EXPECT_EQ("B", (*restoredArray)[1]->getClassName()->utf8());

    using Row = ElementMatrix::SubArray;
    auto matrix = ::jxx::NEW<ElementMatrix>(1);
    auto row = ::jxx::NEW<Row>(1);
    (*row)[0] = element("C", "three", "C.cpp", 3);
    matrix->row(0) = row;

    auto restoredMatrix = roundTrip(matrix);
    ASSERT_NE(restoredMatrix, nullptr);
    const auto& restoredRow = restoredMatrix->row(0);
    ASSERT_NE(restoredRow, nullptr);
    EXPECT_EQ("C", (*restoredRow)[0]->getClassName()->utf8());
}

TEST(StackTraceElementSerialization, NoDataIsRejected) {
    auto value = element("A", "one", "A.cpp", 1);
    EXPECT_THROW(value->readObjectNoData(), ::jxx::io::InvalidObjectException);
}

} // namespace
