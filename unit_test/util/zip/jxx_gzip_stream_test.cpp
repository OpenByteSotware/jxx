#include <gtest/gtest.h>
#include <string>
#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "lang/jxx.lang.String.h"
#include "util/zip/jxx.util.zip.GZIPInputStream.h"
#include "util/zip/jxx.util.zip.GZIPOutputStream.h"
#include "util/zip/jxx.util.zip.ZipEntry.h"
#include "util/zip/jxx.util.zip.ZipException.h"
#include "util/zip/jxx.util.zip.ZipOutputStream.h"

namespace {

::jxx::lang::ByteArray bytes(const std::string& value) {
    auto result = ::jxx::NEW<::jxx::lang::ByteArrayType>(
        static_cast<::jxx::lang::jint>(value.size()));
    for (::jxx::lang::jint index = 0; index < result->length; ++index) {
        (*result)[index] = static_cast<::jxx::lang::jbyte>(
            value[static_cast<std::size_t>(index)]);
    }
    return result;
}

TEST(JxxGzipStreamTest, RoundTripAndEmpty) {
    for (const std::string text : {
             std::string(),
             std::string("gzip parity data")}) {
        auto sink = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
        auto output = ::jxx::NEW<::jxx::util::zip::GZIPOutputStream>(sink);
        output->write(bytes(text));
        output->finish();

        auto input = ::jxx::NEW<::jxx::util::zip::GZIPInputStream>(
            ::jxx::NEW<::jxx::io::ByteArrayInputStream>(
                sink->toByteArray()));
        auto result = ::jxx::NEW<::jxx::lang::ByteArrayType>(
            static_cast<::jxx::lang::jint>(text.size() + 1U));
        const auto count = input->read(result);
        EXPECT_EQ(
            static_cast<::jxx::lang::jint>(text.size()),
            count < 0 ? 0 : count);
        EXPECT_EQ(-1, input->read());
    }
}

TEST(JxxGzipStreamTest, TrailerCrcIsValidated) {
    auto sink = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    auto output = ::jxx::NEW<::jxx::util::zip::GZIPOutputStream>(sink);
    output->write(bytes("bad"));
    output->finish();

    auto data = sink->toByteArray();
    (*data)[data->length - 8] ^= 1;

    auto input = ::jxx::NEW<::jxx::util::zip::GZIPInputStream>(
        ::jxx::NEW<::jxx::io::ByteArrayInputStream>(data));
    auto buffer = ::jxx::NEW<::jxx::lang::ByteArrayType>(16);
    EXPECT_GT(input->read(buffer), 0);
    EXPECT_THROW(input->read(buffer), ::jxx::util::zip::ZipException);
}

TEST(JxxGzipStreamTest, ConcatenatedMembers) {
    auto sink = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();

    for (const char* value : {"a", "b"}) {
        auto member = ::jxx::NEW<::jxx::util::zip::GZIPOutputStream>(
            ::jxx::CAST<::jxx::io::OutputStream>(sink));
        member->write(bytes(value));
        member->finish();
    }

    auto input = ::jxx::NEW<::jxx::util::zip::GZIPInputStream>(
        ::jxx::NEW<::jxx::io::ByteArrayInputStream>(
            sink->toByteArray()));
    EXPECT_EQ('a', input->read());
    EXPECT_EQ('b', input->read());
    EXPECT_EQ(-1, input->read());
}

TEST(JxxStoredMetadataTest, InfersMissingStoredSize) {
    auto sink = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    auto output = ::jxx::NEW<::jxx::util::zip::ZipOutputStream>(sink);
    auto entry = ::jxx::NEW<::jxx::util::zip::ZipEntry>(
        ::jxx::NEW<::jxx::lang::String>("x"));
    entry->setMethod(::jxx::util::zip::ZipEntry::STORED);
    entry->setSize(0);
    entry->setCrc(0);

    EXPECT_NO_THROW(output->putNextEntry(entry));
    EXPECT_EQ(0, entry->getCompressedSize());
}

} // namespace
