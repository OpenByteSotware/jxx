#include "com/google/gson/jxx.com.google.gson.Gson.h"

#include "com/google/gson/jxx.com.google.gson.JsonElement.h"
#include "com/google/gson/jxx.com.google.gson.JsonNull.h"
#include "com/google/gson/jxx.com.google.gson.JsonParser.h"
#include "com/google/gson/jxx.com.google.gson.TypeAdapter.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace com::google::gson {

::jxx::Ptr<JsonElement> Gson::fromJson(
    const ::jxx::Ptr<::jxx::lang::String>& json) const {
    return JsonParser::parseString(json);
}

::jxx::Ptr<::jxx::lang::String> Gson::toJson(
    const ::jxx::Ptr<JsonElement>& element) const {
    return (element == nullptr ? JsonNull::INSTANCE() : element)->toString();
}

::jxx::Ptr<::jxx::lang::Object> Gson::fromJson(
    const ::jxx::Ptr<::jxx::lang::String>& json,
    const ::jxx::Ptr<TypeAdapter>& adapter) const {
    if (adapter == nullptr) throw ::jxx::lang::NullPointerException();
    return adapter->fromJsonTree(JsonParser::parseString(json));
}

::jxx::Ptr<::jxx::lang::String> Gson::toJson(
    const ::jxx::Ptr<::jxx::lang::Object>& value,
    const ::jxx::Ptr<TypeAdapter>& adapter) const {
    if (adapter == nullptr) throw ::jxx::lang::NullPointerException();
    return toJson(adapter->toJsonTree(value));
}

} // namespace com::google::gson
