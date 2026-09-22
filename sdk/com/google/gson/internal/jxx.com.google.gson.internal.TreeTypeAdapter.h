#pragma once
#include "com/google/gson/jxx.com.google.gson.JsonDeserializer.h"
#include "com/google/gson/jxx.com.google.gson.JsonSerializer.h"
#include "com/google/gson/jxx.com.google.gson.TypeAdapter.h"
namespace com::google::gson::internal {
class TreeTypeAdapter final
    : public ::jxx::lang::ClassBase<TreeTypeAdapter,::jxx::lang::Object,::com::google::gson::TypeAdapter> {
public:
 TreeTypeAdapter(const ::jxx::Ptr<::com::google::gson::JsonSerializer>& serializer,
                 const ::jxx::Ptr<::com::google::gson::JsonDeserializer>& deserializer,
                 const ::jxx::Ptr<::jxx::lang::ClassAny>& type,
                 const ::jxx::Ptr<::com::google::gson::JsonSerializationContext>& serializationContext,
                 const ::jxx::Ptr<::com::google::gson::JsonDeserializationContext>& deserializationContext);
 ::jxx::Ptr<::jxx::lang::Object> fromJsonTree(const ::jxx::Ptr<::com::google::gson::JsonElement>& element) override;
 ::jxx::Ptr<::com::google::gson::JsonElement> toJsonTree(const ::jxx::Ptr<::jxx::lang::Object>& value) override;
private:
 ::jxx::Ptr<::com::google::gson::JsonSerializer> serializer_;
 ::jxx::Ptr<::com::google::gson::JsonDeserializer> deserializer_;
 ::jxx::Ptr<::jxx::lang::ClassAny> type_;
 ::jxx::Ptr<::com::google::gson::JsonSerializationContext> serializationContext_;
 ::jxx::Ptr<::com::google::gson::JsonDeserializationContext> deserializationContext_;
};}
