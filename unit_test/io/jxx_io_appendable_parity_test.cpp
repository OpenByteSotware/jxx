#include <type_traits>

#include <gtest/gtest.h>

#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.PrintStream.h"
#include "io/jxx.io.Writer.h"
#include "lang/jxx.lang.Appendable.h"
#include "lang/jxx.lang.String.h"

namespace {

TEST(IoAppendableParity, WriterImplementsAppendable) {
    EXPECT_TRUE((std::is_base_of_v<
        ::jxx::lang::Appendable,
        ::jxx::io::Writer>));
}

TEST(IoAppendableParity, PrintStreamImplementsAppendable) {
    EXPECT_TRUE((std::is_base_of_v<
        ::jxx::lang::Appendable,
        ::jxx::io::PrintStream>));
}

TEST(IoAppendableParity, PrintStreamAppendWritesTextAndReturnsSameObject) {
    const auto bytes = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    const auto stream = ::jxx::NEW<::jxx::io::PrintStream>(bytes);
    const auto text = ::jxx::NEW<::jxx::lang::String>("abcdef");

    const auto result = stream->append(text, 1, 4);
    result->append(static_cast<::jxx::lang::jchar>('!'));
    stream->append(nullptr);
    stream->flush();

    EXPECT_EQ(result.get(),
              ::jxx::CAST<::jxx::lang::Appendable>(stream).get());
    EXPECT_EQ("bcd!null", bytes->toString()->utf8());
}

} // namespace
