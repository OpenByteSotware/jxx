#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.buildin_array.h"
#include "lang/jxx.lang.Class.h"
#include "com/google/gson/jxx.com.google.gson.FieldNamingStrategy.h"
#include "com/google/gson/jxx.com.google.gson.InstanceCreator.h"
#include "com/google/gson/jxx.com.google.gson.TypeAdapterFactory.h"
#include "com/google/gson/jxx.com.google.gson.ExclusionStrategy.h"
#include "com/google/gson/jxx.com.google.gson.LongSerializationPolicy.h"
#include "com/google/gson/jxx.com.google.gson.ToNumberStrategy.h"
#include "com/google/gson/jxx.com.google.gson.JsonSerializer.h"
#include "com/google/gson/jxx.com.google.gson.JsonDeserializer.h"
#include "com/google/gson/reflect/jxx.com.google.gson.reflect.TypeToken.h"

#include <string>

namespace jxx::lang { class String; }
namespace jxx::io { class Reader; class Writer; }
namespace com::google::gson::stream { class JsonReader; class JsonWriter; }
namespace com::google::gson { class JsonElement; class TypeAdapter; }

namespace com::google::gson {

class Gson final
    : public ::jxx::lang::ClassBase<Gson, ::jxx::lang::Object> {
public:
    Gson() = default;
    Gson(
        ::jxx::lang::jbool serializeNulls,
        ::jxx::lang::jbool htmlSafe,
        ::jxx::lang::jbool prettyPrinting,
        ::jxx::lang::jbool lenient,
        const ::jxx::Ptr<FieldNamingStrategy>& fieldNamingStrategy = nullptr,
        const ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::ClassAny>, 1U>>& adapterTypes = nullptr,
        const ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<TypeAdapter>, 1U>>& adapters = nullptr,
        const ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::ClassAny>, 1U>>& creatorTypes = nullptr,
        const ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<InstanceCreator>, 1U>>& creators = nullptr,
        const ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<TypeAdapterFactory>, 1U>>& factories = nullptr,
        const ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<ExclusionStrategy>, 1U>>& serializationStrategies = nullptr,
        const ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<ExclusionStrategy>, 1U>>& deserializationStrategies = nullptr,
        ::jxx::lang::jbool requireExpose = false,
        ::jxx::lang::jdouble version = -1.0,
        ::jxx::lang::jbool nonExecutableJson = false,
        ::jxx::lang::jbool specialFloatingPointValues = false,
        LongSerializationPolicy longSerializationPolicy = LongSerializationPolicy::DEFAULT,
        const ::jxx::Ptr<ToNumberStrategy>& objectToNumberStrategy = nullptr,
        const ::jxx::Ptr<ToNumberStrategy>& numberToNumberStrategy = nullptr,
        const ::jxx::Ptr<::jxx::lang::String>& dateFormatPattern = nullptr,
        ::jxx::lang::jint dateStyle = -1,
        ::jxx::lang::jbool serializeInnerClasses = true,
        ::jxx::lang::jbool complexMapKeySerialization = false,
        const ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::ClassAny>, 1U>>& serializerTypes = nullptr,
        const ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<JsonSerializer>, 1U>>& serializers = nullptr,
        const ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::ClassAny>, 1U>>& deserializerTypes = nullptr,
        const ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<JsonDeserializer>, 1U>>& deserializers = nullptr);

    ::jxx::Ptr<JsonElement> fromJson(
        const ::jxx::Ptr<::jxx::lang::String>& json) const;
    ::jxx::Ptr<JsonElement> fromJson(const ::jxx::Ptr<::jxx::io::Reader>& reader) const;
    ::jxx::Ptr<JsonElement> fromJson(const ::jxx::Ptr<::com::google::gson::stream::JsonReader>& reader) const;

    ::jxx::Ptr<::jxx::lang::String> toJson(
        const ::jxx::Ptr<JsonElement>& element) const;
    void toJson(const ::jxx::Ptr<JsonElement>& element, const ::jxx::Ptr<::jxx::io::Writer>& writer) const;
    void toJson(const ::jxx::Ptr<JsonElement>& element, const ::jxx::Ptr<::com::google::gson::stream::JsonWriter>& writer) const;

    ::jxx::Ptr<::jxx::lang::Object> fromJson(
        const ::jxx::Ptr<::jxx::lang::String>& json,
        const ::jxx::Ptr<TypeAdapter>& adapter) const;
    ::jxx::Ptr<::jxx::lang::Object> fromJson(
        const ::jxx::Ptr<::jxx::lang::String>& json,
        const ::jxx::Ptr<::com::google::gson::reflect::TypeToken>& type) const;

    ::jxx::Ptr<::jxx::lang::String> toJson(
        const ::jxx::Ptr<::jxx::lang::Object>& value,
        const ::jxx::Ptr<TypeAdapter>& adapter) const;
    ::jxx::Ptr<::jxx::lang::String> toJson(
        const ::jxx::Ptr<::jxx::lang::Object>& value,
        const ::jxx::Ptr<::jxx::lang::ClassAny>& type) const;
    ::jxx::Ptr<::jxx::lang::String> toJson(
        const ::jxx::Ptr<::jxx::lang::Object>& value,
        const ::jxx::Ptr<::com::google::gson::reflect::TypeToken>& type) const;

    ::jxx::lang::jbool serializeNulls() const noexcept;
    ::jxx::lang::jbool htmlSafe() const noexcept;
    ::jxx::lang::jbool prettyPrinting() const noexcept;
    ::jxx::lang::jbool lenient() const noexcept;
    ::jxx::Ptr<FieldNamingStrategy> fieldNamingStrategy() const;
    ::jxx::Ptr<TypeAdapter> getAdapter(
        const ::jxx::Ptr<::jxx::lang::ClassAny>& type) const;
    ::jxx::Ptr<TypeAdapter> getAdapter(
        const ::jxx::Ptr<::com::google::gson::reflect::TypeToken>& type) const;
    ::jxx::Ptr<::jxx::lang::Object> createInstance(
        const ::jxx::Ptr<::jxx::lang::ClassAny>& type) const;
    ::jxx::lang::jbool shouldSkipField(
        const ::jxx::Ptr<FieldAttributes>& field,
        ::jxx::lang::jbool serialization) const;
    ::jxx::lang::jbool shouldSkipClass(
        const ::jxx::Ptr<::jxx::lang::ClassAny>& type,
        ::jxx::lang::jbool serialization) const;
    ::jxx::lang::jbool requireExpose() const noexcept;
    ::jxx::lang::jdouble version() const noexcept;
    LongSerializationPolicy longSerializationPolicy() const noexcept;
    ::jxx::Ptr<ToNumberStrategy> objectToNumberStrategy() const;
    ::jxx::Ptr<ToNumberStrategy> numberToNumberStrategy() const;
    ::jxx::Ptr<::jxx::lang::String> dateFormatPattern() const;
    ::jxx::lang::jint dateStyle() const noexcept;
    ::jxx::lang::jbool serializeInnerClasses() const noexcept;
    ::jxx::lang::jbool complexMapKeySerialization() const noexcept;
    ::jxx::Ptr<JsonSerializer> getSerializer(const ::jxx::Ptr<::jxx::lang::ClassAny>& type) const;
    ::jxx::Ptr<JsonDeserializer> getDeserializer(const ::jxx::Ptr<::jxx::lang::ClassAny>& type) const;

private:
    static std::string formatTree_(
        const ::jxx::Ptr<JsonElement>& element,
        ::jxx::lang::jbool serializeNulls,
        ::jxx::lang::jbool htmlSafe,
        ::jxx::lang::jbool prettyPrinting,
        ::jxx::lang::jint depth);

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
    ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::ClassAny>, 1U>> serializerTypes_;
    ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<JsonSerializer>, 1U>> serializers_;
    ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::ClassAny>, 1U>> deserializerTypes_;
    ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<JsonDeserializer>, 1U>> deserializers_;
};

} // namespace com::google::gson
