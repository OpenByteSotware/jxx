#include <gtest/gtest.h>

#include "io/jxx.io.BufferedReader.h"
#include "io/jxx.io.IOException.h"
#include "io/jxx.io.StringReader.h"
#include "lang/jxx.lang.String.h"

TEST(BufferedReaderMarkParity, ResetWithinReadAheadLimit) {
    auto reader = ::jxx::NEW<::jxx::io::BufferedReader>(
        ::jxx::NEW<::jxx::io::StringReader>(
            ::jxx::NEW<::jxx::lang::String>("abcdef")), 2);
    reader->mark(4);
    EXPECT_EQ('a', reader->read());
    EXPECT_EQ('b', reader->read());
    EXPECT_EQ('c', reader->read());
    reader->reset();
    EXPECT_EQ('a', reader->read());
}

TEST(BufferedReaderMarkParity, InvalidatesMarkAfterLimit) {
    auto reader = ::jxx::NEW<::jxx::io::BufferedReader>(
        ::jxx::NEW<::jxx::io::StringReader>(
            ::jxx::NEW<::jxx::lang::String>("abcdef")), 2);
    reader->mark(2);
    EXPECT_EQ('a', reader->read());
    EXPECT_EQ('b', reader->read());
    EXPECT_EQ('c', reader->read());
    EXPECT_THROW(reader->reset(), ::jxx::io::IOException);
}

TEST(BufferedReaderMarkParity, ClosedReaderOperationsThrowIOException) {
    auto reader = ::jxx::NEW<::jxx::io::BufferedReader>(
        ::jxx::NEW<::jxx::io::StringReader>(
            ::jxx::NEW<::jxx::lang::String>("a")));
    reader->close();
    EXPECT_THROW(reader->read(), ::jxx::io::IOException);
    EXPECT_THROW(reader->ready(), ::jxx::io::IOException);
    EXPECT_THROW(reader->mark(1), ::jxx::io::IOException);
    EXPECT_THROW(reader->reset(), ::jxx::io::IOException);
}
