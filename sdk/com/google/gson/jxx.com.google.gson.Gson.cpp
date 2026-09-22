#include "com/google/gson/jxx.com.google.gson.Gson.h"

#include "com/google/gson/jxx.com.google.gson.JsonElement.h"
#include "com/google/gson/jxx.com.google.gson.JsonNull.h"
#include "com/google/gson/jxx.com.google.gson.JsonParser.h"

namespace com::google::gson {

::jxx::Ptr<JsonElement> Gson::fromJson(
    const ::jxx::Ptr<::jxx::lang::String>& json) const {
    return JsonParser::parseString(json);
}

::jxx::Ptr<::jxx::lang::String> Gson::toJson(
    const ::jxx::Ptr<JsonElement>& element) const {
    return (element == nullptr ? JsonNull::INSTANCE() : element)->toString();
}

} // namespace com::google::gson
