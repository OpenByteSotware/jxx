#include <gtest/gtest.h>

#include "jxx.initialize.h"
#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.File.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "lang/jxx.lang.String.h"

namespace {

template <typename T>
::jxx::Ptr<T> roundTrip(const ::jxx::Ptr<T>& value) {
    auto bytes = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    auto output = ::jxx::NEW<::jxx::io::ObjectOutputStream>(
        ::jxx::CAST<::jxx::io::OutputStream>(bytes));
    output->writeObject(::jxx::CAST<::jxx::lang::Object>(value));
    output->flush();

    auto inputBytes = ::jxx::NEW<::jxx::io::ByteArrayInputStream>(
        bytes->toByteArray());
    auto input = ::jxx::NEW<::jxx::io::ObjectInputStream>(
        ::jxx::CAST<::jxx::io::InputStream>(inputBytes));
    return ::jxx::CAST<T>(input->readObject());
}

TEST(CoreSerializationRegistration, RoundTripsString) {
    ::jxx::initialize();
    const auto restored = roundTrip(::jxx::NEW<::jxx::lang::String>("serialization text"));
    ASSERT_NE(restored, nullptr);
    EXPECT_EQ(restored->utf8(), "serialization text");
}

TEST(CoreSerializationRegistration, RoundTripsFilePath) {
    ::jxx::initialize();
    const auto restored = roundTrip(::jxx::NEW<::jxx::io::File>(
        ::jxx::NEW<::jxx::lang::String>("relative/path.txt")));
    ASSERT_NE(restored, nullptr);
    ASSERT_NE(restored->getPath(), nullptr);
    EXPECT_EQ(restored->getPath()->utf8(), "relative/path.txt");
}

} // namespace
