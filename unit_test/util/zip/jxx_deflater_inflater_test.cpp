#include <gtest/gtest.h>

#include <string>

#include "lang/jxx.lang.IllegalStateException.h"
#include "util/zip/jxx.util.zip.DataFormatException.h"
#include "util/zip/jxx.util.zip.Deflater.h"
#include "util/zip/jxx.util.zip.Inflater.h"

namespace {
::jxx::lang::ByteArray makeBytes(const std::string& value) {
    auto result=::jxx::NEW<::jxx::lang::ByteArrayType>(static_cast<::jxx::lang::jint>(value.size()));
    for(::jxx::lang::jint i=0;i<result->length;++i)(*result)[i]=static_cast<::jxx::lang::jbyte>(value[static_cast<std::size_t>(i)]);
    return result;
}
TEST(JxxDeflaterInflaterTest, RoundTripWrappedData) {
    auto source=makeBytes("deflater inflater parity data deflater inflater parity data");
    auto compressed=::jxx::NEW<::jxx::lang::ByteArrayType>(256);
    auto deflater=::jxx::NEW<::jxx::util::zip::Deflater>();deflater->setInput(source);deflater->finish();
    const auto compressedCount=deflater->deflate(compressed);
    EXPECT_TRUE(deflater->finished()); EXPECT_GT(compressedCount,0);
    auto output=::jxx::NEW<::jxx::lang::ByteArrayType>(source->length);
    auto inflater=::jxx::NEW<::jxx::util::zip::Inflater>();inflater->setInput(compressed,0,compressedCount);
    EXPECT_EQ(source->length,inflater->inflate(output)); EXPECT_TRUE(inflater->finished());
    for(::jxx::lang::jint i=0;i<source->length;++i)EXPECT_EQ((*source)[i],(*output)[i]);
}
TEST(JxxDeflaterInflaterTest, RawModeRoundTrip) {
    auto source=makeBytes("raw stream");auto compressed=::jxx::NEW<::jxx::lang::ByteArrayType>(128);
    auto deflater=::jxx::NEW<::jxx::util::zip::Deflater>(::jxx::util::zip::Deflater::DEFAULT_COMPRESSION,true);deflater->setInput(source);deflater->finish();const auto count=deflater->deflate(compressed);
    auto output=::jxx::NEW<::jxx::lang::ByteArrayType>(64);auto inflater=::jxx::NEW<::jxx::util::zip::Inflater>(true);inflater->setInput(compressed,0,count);EXPECT_EQ(source->length,inflater->inflate(output));
}
TEST(JxxDeflaterInflaterTest, InvalidDataThrowsDataFormatException) {
    auto bad=makeBytes("not compressed");auto output=::jxx::NEW<::jxx::lang::ByteArrayType>(64);auto inflater=::jxx::NEW<::jxx::util::zip::Inflater>();inflater->setInput(bad);
    EXPECT_THROW(inflater->inflate(output),::jxx::util::zip::DataFormatException);
}
TEST(JxxDeflaterInflaterTest, EndRejectsFurtherUse) {
    auto inflater=::jxx::NEW<::jxx::util::zip::Inflater>();inflater->end();auto output=::jxx::NEW<::jxx::lang::ByteArrayType>(8);
    EXPECT_THROW(inflater->inflate(output),::jxx::lang::IllegalStateException);
}
} // namespace
