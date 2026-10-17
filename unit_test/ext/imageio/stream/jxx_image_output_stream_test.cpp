#include <gtest/gtest.h>
#include "ext/imageio/stream/jxx.ext.imageio.stream.MemoryCacheImageOutputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
TEST(ImageOutputStreamParity, WritesSeeksAndReadsPrimitiveValues) {
    const auto output = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    const auto stream = ::jxx::NEW<::jxx::ext::imageio::stream::MemoryCacheImageOutputStream>(output);
    stream->writeInt(0x12345678);
    stream->writeBoolean(true);
    EXPECT_EQ(stream->length(), 5);
    stream->seek(0);
    EXPECT_EQ(stream->readInt(), 0x12345678);
    EXPECT_TRUE(stream->readBoolean());
}
TEST(ImageOutputStreamParity, SupportsBitsMarksAndCacheQueries) {
    const auto output = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    const auto stream = ::jxx::NEW<::jxx::ext::imageio::stream::MemoryCacheImageOutputStream>(output);
    stream->writeBits(0x5, 3);
    stream->seek(0);
    EXPECT_EQ(stream->readBits(3), 0x5);
    stream->seek(0);
    stream->mark();
    stream->readBit();
    stream->reset();
    EXPECT_EQ(stream->getStreamPosition(), 0);
    EXPECT_TRUE(stream->isCached());
    EXPECT_TRUE(stream->isCachedMemory());
    EXPECT_FALSE(stream->isCachedFile());
}
