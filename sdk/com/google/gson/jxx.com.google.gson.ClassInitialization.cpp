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

namespace com::google::gson {
namespace {

template <typename T>
void initializeType()
{
    (void)T::Class();
}

} // namespace

void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
        initializeType<ExclusionStrategy>();
        initializeType<FieldAttributes>();
        initializeType<FieldNamingStrategy>();
        initializeType<Gson>();
        initializeType<GsonBuilder>();
        initializeType<InstanceCreator>();
        initializeType<JsonArray>();
        initializeType<JsonDeserializationContext>();
        initializeType<JsonDeserializer>();
        initializeType<JsonElement>();
        initializeType<JsonIOException>();
        initializeType<JsonNull>();
        initializeType<JsonObject>();
        initializeType<JsonParseException>();
        initializeType<JsonParser>();
        initializeType<JsonPrimitive>();
        initializeType<JsonSerializationContext>();
        initializeType<JsonSerializer>();
        initializeType<JsonStreamParser>();
        initializeType<JsonSyntaxException>();
        initializeType<ToNumberStrategy>();
        initializeType<TypeAdapter>();
        initializeType<TypeAdapterFactory>();
    });
}

} // namespace com::google::gson
