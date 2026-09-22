#pragma once
#include "com/google/gson/jxx.com.google.gson.JsonDeserializationContext.h"
namespace com::google::gson {
class JsonDeserializer
    : public ::jxx::lang::InterfaceBase<JsonDeserializer> {
public:
    ~JsonDeserializer() override = default;
    virtual ::jxx::Ptr<::jxx::lang::Object> deserialize(
        const ::jxx::Ptr<JsonElement>& json,
        const ::jxx::Ptr<::jxx::lang::ClassAny>& type,
        const ::jxx::Ptr<JsonDeserializationContext>& context) = 0;
};
} // namespace com::google::gson
