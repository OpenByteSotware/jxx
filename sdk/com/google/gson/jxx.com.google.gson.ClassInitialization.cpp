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
        (void)FieldNamingPolicy::JxxClassInfoMarker::Class();
        (void)LongSerializationPolicy::JxxClassInfoMarker::Class();
        (void)ToNumberPolicy::JxxClassInfoMarker::Class();
    });
}

} // namespace com::google::gson
