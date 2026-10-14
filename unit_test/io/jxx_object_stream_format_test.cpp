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
