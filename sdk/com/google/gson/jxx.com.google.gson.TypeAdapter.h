#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "com/google/gson/stream/jxx.com.google.gson.stream.JsonReader.h"
#include "com/google/gson/stream/jxx.com.google.gson.stream.JsonWriter.h"

namespace com::google::gson {
class JsonElement;

class TypeAdapter
    : public ::jxx::lang::InterfaceBase<TypeAdapter> {
public:
    ~TypeAdapter() override = default;

    virtual ::jxx::Ptr<::jxx::lang::Object> fromJsonTree(
        const ::jxx::Ptr<JsonElement>& element) = 0;

    virtual ::jxx::Ptr<JsonElement> toJsonTree(
        const ::jxx::Ptr<::jxx::lang::Object>& value) = 0;

    virtual ::jxx::Ptr<::jxx::lang::Object> read(
        const ::jxx::Ptr<::com::google::gson::stream::JsonReader>& reader);
    virtual void write(
        const ::jxx::Ptr<::com::google::gson::stream::JsonWriter>& writer,
        const ::jxx::Ptr<::jxx::lang::Object>& value);
    ::jxx::Ptr<::jxx::lang::Object> fromJson(
        const ::jxx::Ptr<::jxx::lang::String>& json);
    ::jxx::Ptr<::jxx::lang::String> toJson(
        const ::jxx::Ptr<::jxx::lang::Object>& value);
    static ::jxx::Ptr<TypeAdapter> nullSafe(
        const ::jxx::Ptr<TypeAdapter>& delegate);
};

} // namespace com::google::gson
