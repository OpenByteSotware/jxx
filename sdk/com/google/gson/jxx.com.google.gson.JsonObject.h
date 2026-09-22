#pragma once
#include "com/google/gson/jxx.com.google.gson.JsonElement.h"
#include "com/google/gson/internal/jxx.com.google.gson.internal.LinkedTreeMap.h"
#include "lang/jxx.lang.String.h"
namespace com::google::gson {
class JsonObject final : public ::jxx::lang::ClassBase<JsonObject, JsonElement> {
public:
    JsonObject();
    ::jxx::lang::jbool isJsonObject() const noexcept override { return true; }
    void add(const ::jxx::Ptr<::jxx::lang::String>& property,const ::jxx::Ptr<JsonElement>& value);
    void addProperty(const ::jxx::Ptr<::jxx::lang::String>& property,const ::jxx::Ptr<::jxx::lang::String>& value);
    ::jxx::Ptr<JsonElement> get(const ::jxx::Ptr<::jxx::lang::String>& property) const;
    ::jxx::lang::jbool has(const ::jxx::Ptr<::jxx::lang::String>& property) const;
    ::jxx::Ptr<::jxx::lang::String> toString() const override;
private: ::jxx::Ptr<::com::google::gson::internal::LinkedTreeMap<::jxx::lang::String,JsonElement>> members_;
};
} // namespace com::google::gson
