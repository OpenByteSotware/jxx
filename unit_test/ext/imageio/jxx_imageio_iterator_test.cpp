#include <gtest/gtest.h>

#include "ext/imageio/jxx.ext.imageio.ImageIO.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.Iterator.h"
#include "util/jxx.util.NoSuchElementException.h"

TEST(ImageIOParity, WriterIteratorThrowsWhenExhausted)
{
    auto iterator = ::jxx::ext::imageio::ImageIO::getImageWritersByFormatName(
        ::jxx::NEW<::jxx::lang::String>("JPEG"));
    ASSERT_NE(iterator, nullptr);
    EXPECT_TRUE(iterator->hasNext());
    EXPECT_NE(iterator->next(), nullptr);
    EXPECT_FALSE(iterator->hasNext());
    EXPECT_THROW(iterator->next(), ::jxx::util::NoSuchElementException);
}

TEST(ImageIOParity, UnknownFormatReturnsEmptyIterator)
{
    auto iterator = ::jxx::ext::imageio::ImageIO::getImageWritersByFormatName(
        ::jxx::NEW<::jxx::lang::String>("unknown"));
    ASSERT_NE(iterator, nullptr);
    EXPECT_FALSE(iterator->hasNext());
    EXPECT_THROW(iterator->next(), ::jxx::util::NoSuchElementException);
}

TEST(ImageIOParity, RejectsNullArguments)
{
    EXPECT_THROW(
        ::jxx::ext::imageio::ImageIO::getImageWritersByFormatName(nullptr),
        ::jxx::lang::IllegalArgumentException);
    EXPECT_THROW(::jxx::ext::imageio::ImageIO::createImageOutputStream(nullptr),
        ::jxx::lang::IllegalArgumentException);
}
