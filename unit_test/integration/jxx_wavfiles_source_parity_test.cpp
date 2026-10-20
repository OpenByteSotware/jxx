#include <gtest/gtest.h>

#include "jxx.initialize.h"
#include "io/jxx.io.BufferedReader.h"
#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.InputStreamReader.h"
#include "lang/jxx.lang.Integer.h"
#include "lang/jxx.lang.String.h"
#include "nio/charset/jxx.nio.charset.Charset.h"
#include "util/jxx.util.Arrays.h"

namespace {

::jxx::lang::ByteArray bytesOf(const char* text)
{
    const std::string value(text);
    auto bytes = ::jxx::NEW<::jxx::lang::ByteArrayType>(
        static_cast<std::uint32_t>(value.size()));
    for (std::size_t index = 0; index < value.size(); ++index)
        (*bytes)[index] = static_cast<::jxx::lang::jbyte>(value[index]);
    return bytes;
}

TEST(WavFilesSourceParity, Utf8ReaderAndSplitPopulateOverrideData)
{
    ::jxx::initialize();
    auto input = ::jxx::NEW<::jxx::io::ByteArrayInputStream>(
        bytesOf("-1=uno,negativo\n1R=street,level,lobby\n"));
    auto charset = ::jxx::nio::charset::Charset::forName(
        ::jxx::NEW<::jxx::lang::String>("UTF-8"));
    auto streamReader = ::jxx::NEW<::jxx::io::InputStreamReader>(
        input, charset);
    auto reader = ::jxx::NEW<::jxx::io::BufferedReader>(streamReader);

    auto first = reader->readLine();
    auto firstParts = first->split(
        ::jxx::NEW<::jxx::lang::String>("="));
    ASSERT_EQ(firstParts->size(), 2U);
    EXPECT_EQ((*firstParts)[0]->utf8(), "-1");
    EXPECT_EQ((*firstParts)[1]->utf8(), "uno,negativo");
    EXPECT_EQ(::jxx::lang::Integer::parseInt((*firstParts)[0]), -1);

    auto phrases = (*firstParts)[1]->split(
        ::jxx::NEW<::jxx::lang::String>(","), 15);
    ASSERT_EQ(phrases->size(), 2U);
    EXPECT_EQ((*phrases)[0]->utf8(), "uno");
    EXPECT_EQ((*phrases)[1]->utf8(), "negativo");

    auto second = reader->readLine();
    auto secondParts = second->split(
        ::jxx::NEW<::jxx::lang::String>("="));
    auto floor = (*secondParts)[0]->toLowerCase()->replaceAll(
        ::jxx::NEW<::jxx::lang::String>("r"),
        ::jxx::NEW<::jxx::lang::String>(""));
    EXPECT_EQ(::jxx::lang::Integer::parseInt(floor), 1);
    EXPECT_EQ(reader->readLine(), nullptr);
    reader->close();
}

TEST(WavFilesSourceParity, AdatsFillAndCopyUseRankOneBracketAccess)
{
    ::jxx::initialize();
    auto inputTable = ::jxx::NEW<::jxx::lang::IntArrayType>(256);
    ::jxx::util::Arrays::fill<::jxx::lang::jint>(inputTable, -128);
    (*inputTable)[0] = 12;
    (*inputTable)[1] = -1;
    (*inputTable)[2] = 33;

    auto adjusted = ::jxx::util::Arrays::copyOfRange<::jxx::lang::jint>(
        inputTable, 0, 3);
    ASSERT_EQ(adjusted->size(), 3U);
    EXPECT_EQ((*adjusted)[0], 12);
    EXPECT_EQ((*adjusted)[1], -1);
    EXPECT_EQ((*adjusted)[2], 33);
}

TEST(WavFilesSourceParity, SplitLimitRetainsRemainder)
{
    ::jxx::initialize();
    auto value = ::jxx::NEW<::jxx::lang::String>("a,b,c,d");
    auto parts = value->split(
        ::jxx::NEW<::jxx::lang::String>(","), 3);
    ASSERT_EQ(parts->size(), 3U);
    EXPECT_EQ((*parts)[0]->utf8(), "a");
    EXPECT_EQ((*parts)[1]->utf8(), "b");
    EXPECT_EQ((*parts)[2]->utf8(), "c,d");
}

} // namespace
