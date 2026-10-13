#include <gtest/gtest.h>

#include "jxx.initialize.h"
#include "com/google/gson/annotations/jxx.com.google.gson.annotations.ClassInitialization.h"
#include "com/google/gson/annotations/jxx.com.google.gson.annotations.Expose.h"
#include "com/google/gson/annotations/jxx.com.google.gson.annotations.JsonAdapter.h"
#include "com/google/gson/annotations/jxx.com.google.gson.annotations.SerializedName.h"
#include "com/google/gson/annotations/jxx.com.google.gson.annotations.Since.h"
#include "com/google/gson/annotations/jxx.com.google.gson.annotations.Until.h"

namespace T = ::com::google::gson::annotations;

TEST(JxxStage52ClassInitialization, MetadataAndIdempotence)
{
    ::jxx::initialize();
    using namespace ::com::google::gson::annotations;
    EXPECT_NE(Expose::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(JsonAdapter::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(SerializedName::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(Since::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(Until::JxxClassInfoMarker::Class(), nullptr);
    const auto before = Expose::JxxClassInfoMarker::Class();
    EXPECT_NO_THROW(T::initializeClasses());
    EXPECT_NO_THROW(T::initializeClasses());
    EXPECT_EQ(before, Expose::JxxClassInfoMarker::Class());
}
