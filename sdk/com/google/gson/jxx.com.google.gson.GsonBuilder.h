#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.buildin_array.h"
#include "com/google/gson/jxx.com.google.gson.FieldNamingPolicy.h"
#include "com/google/gson/jxx.com.google.gson.InstanceCreator.h"
#include "com/google/gson/jxx.com.google.gson.TypeAdapter.h"
#include "com/google/gson/jxx.com.google.gson.TypeAdapterFactory.h"
#include "lang/jxx.lang.Class.h"
#include "com/google/gson/jxx.com.google.gson.ExclusionStrategy.h"
#include "com/google/gson/jxx.com.google.gson.LongSerializationPolicy.h"
#include "com/google/gson/jxx.com.google.gson.ToNumberPolicy.h"
#include "com/google/gson/jxx.com.google.gson.JsonSerializer.h"
#include "com/google/gson/jxx.com.google.gson.JsonDeserializer.h"
#include "lang/jxx.lang.String.h"

namespace com::google::gson {

class Gson;

class GsonBuilder final
    : public ::jxx::lang::ClassBase<GsonBuilder, ::jxx::lang::Object> {
public:
    GsonBuilder() = default;

    ::jxx::Ptr<GsonBuilder> serializeNulls();
    ::jxx::Ptr<GsonBuilder> disableHtmlEscaping();
    ::jxx::Ptr<GsonBuilder> setPrettyPrinting();
    ::jxx::Ptr<GsonBuilder> setLenient();
    ::jxx::Ptr<GsonBuilder> setFieldNamingPolicy(FieldNamingPolicy policy);
    ::jxx::Ptr<GsonBuilder> setFieldNamingStrategy(
        const ::jxx::Ptr<FieldNamingStrategy>& strategy);
    ::jxx::Ptr<GsonBuilder> registerTypeAdapter(
        const ::jxx::Ptr<::jxx::lang::ClassAny>& type,
        const ::jxx::Ptr<TypeAdapter>& adapter);
    ::jxx::Ptr<GsonBuilder> registerInstanceCreator(
        const ::jxx::Ptr<::jxx::lang::ClassAny>& type,
        const ::jxx::Ptr<InstanceCreator>& creator);
    ::jxx::Ptr<GsonBuilder> registerTypeAdapterFactory(
        const ::jxx::Ptr<TypeAdapterFactory>& factory);
    ::jxx::Ptr<GsonBuilder> registerTypeSerializer(
        const ::jxx::Ptr<::jxx::lang::ClassAny>& type,
        const ::jxx::Ptr<JsonSerializer>& serializer);
    ::jxx::Ptr<GsonBuilder> registerTypeDeserializer(
        const ::jxx::Ptr<::jxx::lang::ClassAny>& type,
        const ::jxx::Ptr<JsonDeserializer>& deserializer);
    ::jxx::Ptr<GsonBuilder> addSerializationExclusionStrategy(
        const ::jxx::Ptr<ExclusionStrategy>& strategy);
    ::jxx::Ptr<GsonBuilder> addDeserializationExclusionStrategy(
        const ::jxx::Ptr<ExclusionStrategy>& strategy);
    ::jxx::Ptr<GsonBuilder> setExclusionStrategies(
        const ::jxx::Ptr<::jxx::lang::JxxArray<
            ::jxx::Ptr<ExclusionStrategy>, 1U>>& strategies);
    ::jxx::Ptr<GsonBuilder> excludeFieldsWithoutExposeAnnotation();
    ::jxx::Ptr<GsonBuilder> setVersion(::jxx::lang::jdouble version);
    ::jxx::Ptr<GsonBuilder> generateNonExecutableJson();
    ::jxx::Ptr<GsonBuilder> serializeSpecialFloatingPointValues();
    ::jxx::Ptr<GsonBuilder> setLongSerializationPolicy(
        LongSerializationPolicy policy);
    ::jxx::Ptr<GsonBuilder> setObjectToNumberStrategy(
        const ::jxx::Ptr<ToNumberStrategy>& strategy);
    ::jxx::Ptr<GsonBuilder> setNumberToNumberStrategy(
        const ::jxx::Ptr<ToNumberStrategy>& strategy);
    ::jxx::Ptr<GsonBuilder> setDateFormat(
        const ::jxx::Ptr<::jxx::lang::String>& pattern);
    ::jxx::Ptr<GsonBuilder> setDateFormat(
        ::jxx::lang::jint style);
    ::jxx::Ptr<GsonBuilder> disableInnerClassSerialization();
    ::jxx::Ptr<GsonBuilder> enableComplexMapKeySerialization();

    ::jxx::Ptr<Gson> create() const;

private:
    ::jxx::lang::jbool serializeNulls_ = false;
    ::jxx::lang::jbool htmlSafe_ = true;
    ::jxx::lang::jbool prettyPrinting_ = false;
    ::jxx::lang::jbool lenient_ = false;
    ::jxx::Ptr<FieldNamingStrategy> fieldNamingStrategy_;
    ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::ClassAny>, 1U>> adapterTypes_;
    ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<TypeAdapter>, 1U>> adapters_;
    ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::ClassAny>, 1U>> creatorTypes_;
    ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<InstanceCreator>, 1U>> creators_;
    ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<TypeAdapterFactory>, 1U>> factories_;
    ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::ClassAny>, 1U>> serializerTypes_;
    ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<JsonSerializer>, 1U>> serializers_;
    ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::ClassAny>, 1U>> deserializerTypes_;
    ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<JsonDeserializer>, 1U>> deserializers_;
    ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<ExclusionStrategy>, 1U>> serializationStrategies_;
    ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<ExclusionStrategy>, 1U>> deserializationStrategies_;
    ::jxx::lang::jbool requireExpose_ = false;
    ::jxx::lang::jdouble version_ = -1.0;
    ::jxx::lang::jbool nonExecutableJson_ = false;
    ::jxx::lang::jbool specialFloatingPointValues_ = false;
    LongSerializationPolicy longSerializationPolicy_ = LongSerializationPolicy::DEFAULT;
    ::jxx::Ptr<ToNumberStrategy> objectToNumberStrategy_;
    ::jxx::Ptr<ToNumberStrategy> numberToNumberStrategy_;
    ::jxx::Ptr<::jxx::lang::String> dateFormatPattern_;
    ::jxx::lang::jint dateStyle_ = -1;
    ::jxx::lang::jbool serializeInnerClasses_ = true;
    ::jxx::lang::jbool complexMapKeySerialization_ = false;
};

} // namespace com::google::gson
