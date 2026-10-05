#include <gtest/gtest.h>

#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "util/zip/jxx.util.zip.Adler32.h"
#include "util/zip/jxx.util.zip.CRC32.h"
#include "util/zip/jxx.util.zip.CheckedInputStream.h"
#include "util/zip/jxx.util.zip.CheckedOutputStream.h"

namespace {

::jxx::lang::ByteArray bytes(std::initializer_list<::jxx::lang::jbyte> values) {
    auto result = ::jxx::NEW<::jxx::lang::ByteArrayType>(
        static_cast<::jxx::lang::jint>(values.size()));
    ::jxx::lang::jint index = 0;
    for (const auto value : values) {
        (*result)[index++] = value;
    }
    return result;
}

TEST(JxxZipChecksumStreamTest, Adler32MatchesKnownVector) {
    auto checksum = ::jxx::NEW<::jxx::util::zip::Adler32>();
    auto value = bytes({1, 2, 3, 4, 5});
    checksum->update(value);
    EXPECT_EQ(0x00280010LL, checksum->getValue());
}

TEST(JxxZipChecksumStreamTest, CheckedOutputStreamUpdatesChecksum) {
    auto sink = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    auto checksum = ::jxx::NEW<::jxx::util::zip::CRC32>();
    auto checked = ::jxx::NEW<::jxx::util::zip::CheckedOutputStream>(sink, checksum);
    auto value = bytes({1, 2, 3, 4, 5});

    checked->write(value, 1, 3);

    auto expected = ::jxx::NEW<::jxx::util::zip::CRC32>();
    expected->update(value, 1, 3);
    EXPECT_EQ(expected->getValue(), checked->getChecksum()->getValue());
    EXPECT_EQ(3, sink->size());
}

TEST(JxxZipChecksumStreamTest, CheckedInputStreamSkipUpdatesChecksum) {
    auto value = bytes({1, 2, 3, 4, 5});
    auto source = ::jxx::NEW<::jxx::io::ByteArrayInputStream>(value);
    auto checksum = ::jxx::NEW<::jxx::util::zip::CRC32>();
    auto checked = ::jxx::NEW<::jxx::util::zip::CheckedInputStream>(source, checksum);

    EXPECT_EQ(3, checked->skip(3));

    auto expected = ::jxx::NEW<::jxx::util::zip::CRC32>();
    expected->update(value, 0, 3);
    EXPECT_EQ(expected->getValue(), checked->getChecksum()->getValue());
    EXPECT_EQ(4, checked->read());
}

} // namespace
