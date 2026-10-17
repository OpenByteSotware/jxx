#include <gtest/gtest.h>
#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "jxx.initialize.h"
#include "lang/jxx.lang.String.h"

TEST(StringSerialization, RoundTripsUtf16CodeUnits) {
    ::jxx::initialize();
    const auto source = ::jxx::NEW<::jxx::lang::String>(
        std::u16string{u'A', static_cast<char16_t>(0x03A9),
                      static_cast<char16_t>(0xD83D), static_cast<char16_t>(0xDE80)});
    auto bytes = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    auto output = ::jxx::NEW<::jxx::io::ObjectOutputStream>(bytes);
    output->writeObject(source); output->writeObject(source); output->flush();
    auto input = ::jxx::NEW<::jxx::io::ObjectInputStream>(
        ::jxx::NEW<::jxx::io::ByteArrayInputStream>(bytes->toByteArray()));
    const auto first = std::dynamic_pointer_cast<::jxx::lang::String>(input->readObject());
    const auto second = input->readObject();
    ASSERT_NE(first, nullptr);
    EXPECT_EQ(first->utf16(), source->utf16());
    EXPECT_EQ(first.get(), second.get());
}
