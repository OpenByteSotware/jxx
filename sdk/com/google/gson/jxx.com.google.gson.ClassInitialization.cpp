#include "com/google/gson/jxx.com.google.gson.ClassInitialization.h"

#include <mutex>

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
#include "com/google/gson/jxx.com.google.gson.FieldNamingPolicy.h"
#include "com/google/gson/jxx.com.google.gson.LongSerializationPolicy.h"
#include "com/google/gson/jxx.com.google.gson.ToNumberPolicy.h"

#include "com/google/gson/annotations/jxx.com.google.gson.annotations.ClassInitialization.h"
#include "com/google/gson/internal/jxx.com.google.gson.internal.ClassInitialization.h"
#include "com/google/gson/reflect/jxx.com.google.gson.reflect.ClassInitialization.h"
#include "com/google/gson/stream/jxx.com.google.gson.stream.ClassInitialization.h"
namespace com::google::gson {

void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
        (void)ExclusionStrategy::Class();
        (void)FieldAttributes::Class();
        (void)FieldNamingStrategy::Class();
        (void)Gson::Class();
        (void)GsonBuilder::Class();
        (void)InstanceCreator::Class();
        (void)JsonArray::Class();
        (void)JsonDeserializationContext::Class();
        (void)JsonDeserializer::Class();
        (void)JsonElement::Class();
        (void)JsonIOException::Class();
        (void)JsonNull::Class();
        (void)JsonObject::Class();
        (void)JsonParseException::Class();
        (void)JsonParser::Class();
        (void)JsonPrimitive::Class();
        (void)JsonSerializationContext::Class();
        (void)JsonSerializer::Class();
        (void)::jxx::lang::ClassBase<
            JsonStreamParser,
            ::jxx::lang::Object>::Class();
        (void)JsonSyntaxException::Class();
        (void)ToNumberStrategy::Class();
        (void)TypeAdapter::Class();
        (void)TypeAdapterFactory::Class();
        (void)FieldNamingPolicies::Class();
        (void)LongSerializationPolicies::Class();
        (void)ToNumberPolicies::Class();
    });
}


void initialize()
{
    initializeClasses();
    ::com::google::gson::annotations::initialize();
    ::com::google::gson::internal::initialize();
    ::com::google::gson::reflect::initialize();
    ::com::google::gson::stream::initialize();
}

} // namespace com::google::gson
