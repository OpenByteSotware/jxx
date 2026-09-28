#include <gtest/gtest.h>

#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.ChunkedRequestBodySource.h"
#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.IOException.h"

namespace jxx::com::sun::net::httpserver::internal {
namespace {
::jxx::lang::ByteArray bytes(const char* text)
{
    const auto length = static_cast<::jxx::lang::jint>(std::char_traits<char>::length(text));
    auto result = ::jxx::NEW<::jxx::lang::ByteArrayType>(length);
    for (::jxx::lang::jint index = 0; index < length; ++index)
        (*result)[index] = static_cast<::jxx::lang::jbyte>(text[index]);
    return result;
}
} // namespace

TEST(ChunkedRequestBodySourceTests, DecodesPrefixAndInputChunks)
{
    auto prefix = bytes("4\r\nWi");
    auto tail = bytes("ki\r\n5\r\npedia\r\n0\r\n\r\n");
    auto input = ::jxx::NEW<::jxx::io::ByteArrayInputStream>(tail);
    auto source = ::jxx::NEW<ChunkedRequestBodySource>(input, prefix, 0, prefix->length);
    auto output = ::jxx::NEW<::jxx::lang::ByteArrayType>(9);
    ::jxx::lang::jint total = 0;
    while (total < output->length) total += source->read(output, total, output->length - total);
    EXPECT_EQ(source->read(output, 0, 1), -1);
    EXPECT_TRUE(source->isFullyConsumedInternal());
    const std::string decoded(reinterpret_cast<const char*>(output->data()), output->length);
    EXPECT_EQ(decoded, "Wikipedia");
}

TEST(ChunkedRequestBodySourceTests, AcceptsExtensionsAndSafeTrailers)
{
    auto prefix = bytes("");
    auto encoded = bytes("1;name=value\r\nA\r\n0\r\nX-Test: yes\r\n\r\n");
    auto input = ::jxx::NEW<::jxx::io::ByteArrayInputStream>(encoded);
    auto source = ::jxx::NEW<ChunkedRequestBodySource>(input, prefix, 0, 0);
    auto output = ::jxx::NEW<::jxx::lang::ByteArrayType>(1);
    EXPECT_EQ(source->read(output, 0, 1), 1);
    EXPECT_EQ(source->read(output, 0, 1), -1);
    EXPECT_EQ((*output)[0], static_cast<::jxx::lang::jbyte>('A'));
}

TEST(ChunkedRequestBodySourceTests, RejectsForbiddenTrailer)
{
    auto prefix = bytes("");
    auto encoded = bytes("0\r\nContent-Length: 1\r\n\r\n");
    auto input = ::jxx::NEW<::jxx::io::ByteArrayInputStream>(encoded);
    auto source = ::jxx::NEW<ChunkedRequestBodySource>(input, prefix, 0, 0);
    auto output = ::jxx::NEW<::jxx::lang::ByteArrayType>(1);
    EXPECT_THROW(source->read(output, 0, 1), ::jxx::io::IOException);
}

TEST(ChunkedRequestBodySourceTests, ClosePreservesUnreadState)
{
    auto prefix = bytes("1\r\nA\r\n0\r\n\r\n");
    auto empty = bytes("");
    auto input = ::jxx::NEW<::jxx::io::ByteArrayInputStream>(empty);
    auto source = ::jxx::NEW<ChunkedRequestBodySource>(input, prefix, 0, prefix->length);
    source->close();
    EXPECT_TRUE(source->wasClosedInternal());
    EXPECT_FALSE(source->isFullyConsumedInternal());
}

} // namespace jxx::com::sun::net::httpserver::internal
