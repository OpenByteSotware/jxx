#pragma once
#include "com/google/gson/jxx.com.google.gson.JsonElement.h"
#include "com/google/gson/stream/jxx.com.google.gson.stream.JsonReader.h"
#include "com/google/gson/stream/jxx.com.google.gson.stream.JsonWriter.h"
namespace com::google::gson::internal {
class Streams final {
public:
    static ::jxx::Ptr<::com::google::gson::JsonElement> parse(
        const ::jxx::Ptr<::com::google::gson::stream::JsonReader>& reader);
    static void write(
        const ::jxx::Ptr<::com::google::gson::JsonElement>& element,
        const ::jxx::Ptr<::com::google::gson::stream::JsonWriter>& writer);
};
} // namespace com::google::gson::internal
