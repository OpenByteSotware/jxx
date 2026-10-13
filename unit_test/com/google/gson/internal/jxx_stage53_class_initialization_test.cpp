#include <gtest/gtest.h>

#include "jxx.initialize.h"
#include "com/google/gson/internal/jxx.com.google.gson.internal.ClassInitialization.h"
#include "com/google/gson/internal/jxx.com.google.gson.internal.GsonContext.h"
#include "com/google/gson/internal/jxx.com.google.gson.internal.JsonTreeReader.h"
#include "com/google/gson/internal/jxx.com.google.gson.internal.JsonTreeWriter.h"
#include "com/google/gson/internal/jxx.com.google.gson.internal.NullSafeTypeAdapter.h"
#include "com/google/gson/internal/jxx.com.google.gson.internal.Streams.h"
#include "com/google/gson/internal/jxx.com.google.gson.internal.StringWriter.h"
#include "com/google/gson/internal/jxx.com.google.gson.internal.TreeTypeAdapter.h"

namespace T = ::com::google::gson::internal;

TEST(JxxStage53ClassInitialization, MetadataAndIdempotence)
{
    ::jxx::initialize();
    using namespace ::com::google::gson::internal;
    EXPECT_NE(GsonContext::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(JsonTreeReader::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(JsonTreeWriter::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(NullSafeTypeAdapter::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(Streams::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(StringWriter::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(TreeTypeAdapter::JxxClassInfoMarker::Class(), nullptr);
    const auto before = GsonContext::JxxClassInfoMarker::Class();
    EXPECT_NO_THROW(T::initializeClasses());
    EXPECT_NO_THROW(T::initializeClasses());
    EXPECT_EQ(before, GsonContext::JxxClassInfoMarker::Class());
}
