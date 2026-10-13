#include <gtest/gtest.h>

#include "com/google/gson/jxx.com.google.gson.ExclusionStrategy.h"
#include "com/google/gson/jxx.com.google.gson.FieldAttributes.h"
#include "com/google/gson/jxx.com.google.gson.FieldNamingStrategy.h"
#include "com/google/gson/jxx.com.google.gson.Gson.h"
#include "com/google/gson/jxx.com.google.gson.GsonBuilder.h"
#include "com/google/gson/jxx.com.google.gson.InstanceCreator.h"
#include "com/google/gson/jxx.com.google.gson.JsonArray.h"
#include "com/google/gson/jxx.com.google.gson.JsonDeserializationContext.h"
#include "com/google/gson/jxx.com.google.gson.JsonDeserializer.h"
#include "com/google/gson/jxx.com.google.gson.JsonElement.h"
#include "com/google/gson/jxx.com.google.gson.JsonIOException.h"
#include "com/google/gson/jxx.com.google.gson.JsonNull.h"
#include "com/google/gson/jxx.com.google.gson.JsonObject.h"
#include "com/google/gson/jxx.com.google.gson.JsonParseException.h"
#include "com/google/gson/jxx.com.google.gson.JsonParser.h"
#include "com/google/gson/jxx.com.google.gson.JsonPrimitive.h"
#include "com/google/gson/jxx.com.google.gson.JsonSerializationContext.h"
#include "com/google/gson/jxx.com.google.gson.JsonSerializer.h"
#include "com/google/gson/jxx.com.google.gson.JsonStreamParser.h"
#include "com/google/gson/jxx.com.google.gson.JsonSyntaxException.h"
#include "com/google/gson/jxx.com.google.gson.ToNumberStrategy.h"
#include "com/google/gson/jxx.com.google.gson.TypeAdapter.h"
#include "com/google/gson/jxx.com.google.gson.TypeAdapterFactory.h"
#include "com/google/gson/jxx.com.google.gson.ClassInitialization.h"
#include "jxx.initialize.h"

TEST(JxxGsonClassInitialization, PublicTypesHaveClassMetadata)
{
    ::jxx::initialize();

    EXPECT_NE(::com::google::gson::ExclusionStrategy::Class(), nullptr);
    EXPECT_NE(::com::google::gson::FieldAttributes::Class(), nullptr);
    EXPECT_NE(::com::google::gson::FieldNamingStrategy::Class(), nullptr);
    EXPECT_NE(::com::google::gson::Gson::Class(), nullptr);
    EXPECT_NE(::com::google::gson::GsonBuilder::Class(), nullptr);
    EXPECT_NE(::com::google::gson::InstanceCreator::Class(), nullptr);
    EXPECT_NE(::com::google::gson::JsonArray::Class(), nullptr);
    EXPECT_NE(::com::google::gson::JsonDeserializationContext::Class(), nullptr);
    EXPECT_NE(::com::google::gson::JsonDeserializer::Class(), nullptr);
    EXPECT_NE(::com::google::gson::JsonElement::Class(), nullptr);
    EXPECT_NE(::com::google::gson::JsonIOException::Class(), nullptr);
    EXPECT_NE(::com::google::gson::JsonNull::Class(), nullptr);
    EXPECT_NE(::com::google::gson::JsonObject::Class(), nullptr);
    EXPECT_NE(::com::google::gson::JsonParseException::Class(), nullptr);
    EXPECT_NE(::com::google::gson::JsonParser::Class(), nullptr);
    EXPECT_NE(::com::google::gson::JsonPrimitive::Class(), nullptr);
    EXPECT_NE(::com::google::gson::JsonSerializationContext::Class(), nullptr);
    EXPECT_NE(::com::google::gson::JsonSerializer::Class(), nullptr);
    EXPECT_NE(::com::google::gson::JsonStreamParser::Class(), nullptr);
    EXPECT_NE(::com::google::gson::JsonSyntaxException::Class(), nullptr);
    EXPECT_NE(::com::google::gson::ToNumberStrategy::Class(), nullptr);
    EXPECT_NE(::com::google::gson::TypeAdapter::Class(), nullptr);
    EXPECT_NE(::com::google::gson::TypeAdapterFactory::Class(), nullptr);
}

TEST(JxxGsonClassInitialization, RepeatedInitializationIsIdempotent)
{
    const auto gsonBefore = ::com::google::gson::Gson::Class();
    const auto elementBefore = ::com::google::gson::JsonElement::Class();

    EXPECT_NO_THROW(::com::google::gson::initializeClasses());
    EXPECT_NO_THROW(::com::google::gson::initializeClasses());

    EXPECT_EQ(gsonBefore, ::com::google::gson::Gson::Class());
    EXPECT_EQ(elementBefore, ::com::google::gson::JsonElement::Class());
}
