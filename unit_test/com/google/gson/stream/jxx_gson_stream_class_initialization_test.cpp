#include <gtest/gtest.h>

#include "jxx.initialize.h"
#include "com/google/gson/stream/jxx.com.google.gson.stream.ClassInitialization.h"
#include "com/google/gson/stream/jxx.com.google.gson.stream.JsonReader.h"
#include "com/google/gson/stream/jxx.com.google.gson.stream.JsonWriter.h"
#include "com/google/gson/stream/jxx.com.google.gson.stream.MalformedJsonException.h"

TEST(JxxStage50ClassInitialization, MetadataAndIdempotence)
{
    ::jxx::initialize();
    EXPECT_NE((::jxx::lang::ClassBase<JsonReader, ::jxx::lang::Object, ::jxx::io::Closeable>::Class()), nullptr);
    EXPECT_NE((::jxx::lang::ClassBase<JsonWriter, ::jxx::lang::Object, ::jxx::io::Closeable, ::jxx::io::Flushable>::Class()), nullptr);
    EXPECT_NE((::jxx::lang::ClassBase<MalformedJsonException, ::jxx::io::IOException>::Class()), nullptr);
    const auto before = ::jxx::lang::ClassBase<JsonReader, ::jxx::lang::Object, ::jxx::io::Closeable>::Class();
    EXPECT_NO_THROW(::com::google::gson::stream::initializeClasses());
    EXPECT_NO_THROW(::com::google::gson::stream::initializeClasses());
    EXPECT_EQ(before, (::jxx::lang::ClassBase<JsonReader, ::jxx::lang::Object, ::jxx::io::Closeable>::Class()));
}
