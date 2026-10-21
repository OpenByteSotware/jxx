#include <gtest/gtest.h>

#include "jxx.initialize.h"
#include "lang/jxx.lang.JxxArray.h"
#include "util/zip/jxx.util.zip.CRC32.h"

TEST(CRC32Parity, WholeArrayAndRangeProduceExpectedChecksum)
{
    ::jxx::initialize();
    auto bytes = ::jxx::NEW<::jxx::lang::ByteArrayType>(9);
    const char* value = "123456789";
    for (::jxx::lang::jint index = 0; index < 9; ++index)
        (*bytes)[index] = static_cast<::jxx::lang::jbyte>(value[index]);

    auto crc = ::jxx::NEW<::jxx::util::zip::CRC32>();
    crc->update(bytes);
    EXPECT_EQ(crc->getValue(), static_cast<::jxx::lang::jlong>(0xcbf43926ULL));

    crc->reset();
    crc->update(bytes, 0, 4);
    crc->update(bytes, 4, 5);
    EXPECT_EQ(crc->getValue(), static_cast<::jxx::lang::jlong>(0xcbf43926ULL));
}
