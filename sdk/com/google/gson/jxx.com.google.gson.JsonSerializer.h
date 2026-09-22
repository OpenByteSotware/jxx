#pragma once
#include "com/google/gson/jxx.com.google.gson.JsonSerializationContext.h"
namespace com::google::gson {
class JsonSerializer
    : public ::jxx::lang::InterfaceBase<JsonSerializer> {
public:
    ~JsonSerializer() override = default;
    virtual ::jxx::Ptr<JsonElement> serialize(
        const ::jxx::Ptr<::jxx::lang::Object>& source,
        const ::jxx::Ptr<::jxx::lang::ClassAny>& type,
        const ::jxx::Ptr<JsonSerializationContext>& context) = 0;
};
} // namespace com::google::gson
