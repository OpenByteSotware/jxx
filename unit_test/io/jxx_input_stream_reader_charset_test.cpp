#include <gtest/gtest.h>

#include "jxx.initialize.h"
#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.InputStreamReader.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "nio/charset/jxx.nio.charset.Charset.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"

TEST(InputStreamReaderParity, AcceptsCharsetObject)
{
    ::jxx::initialize();
    auto bytes = ::jxx::NEW<::jxx::lang::ByteArrayType>(3);
    (*bytes)[0] = static_cast<::jxx::lang::jbyte>('A');
    (*bytes)[1] = static_cast<::jxx::lang::jbyte>('B');
    (*bytes)[2] = static_cast<::jxx::lang::jbyte>('C');
    auto input = ::jxx::NEW<::jxx::io::ByteArrayInputStream>(bytes);
    auto charset = ::jxx::nio::charset::Charset::forName(
        ::jxx::NEW<::jxx::lang::String>("UTF-8"));
    auto reader = ::jxx::NEW<::jxx::io::InputStreamReader>(input, charset);

    EXPECT_EQ(reader->getEncoding()->utf8(), "UTF-8");
    EXPECT_EQ(reader->read(), 'A');
    EXPECT_EQ(reader->read(), 'B');
    EXPECT_EQ(reader->read(), 'C');
    EXPECT_EQ(reader->read(), -1);
}

TEST(InputStreamReaderParity, RejectsNullCharsetObject)
{
    ::jxx::initialize();
    auto bytes = ::jxx::NEW<::jxx::lang::ByteArrayType>(0);
    auto input = ::jxx::NEW<::jxx::io::ByteArrayInputStream>(bytes);
    const ::jxx::Ptr<::jxx::nio::charset::Charset> charset = nullptr;
    EXPECT_THROW(
        ::jxx::NEW<::jxx::io::InputStreamReader>(input, charset),
        ::jxx::lang::NullPointerException);
}
