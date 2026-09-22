#include "com/google/gson/jxx.com.google.gson.Gson.h"

#include "com/google/gson/jxx.com.google.gson.JsonArray.h"
#include "com/google/gson/jxx.com.google.gson.JsonElement.h"
#include "com/google/gson/jxx.com.google.gson.JsonNull.h"
#include "com/google/gson/jxx.com.google.gson.JsonObject.h"
#include "com/google/gson/jxx.com.google.gson.JsonParser.h"
#include "com/google/gson/jxx.com.google.gson.JsonPrimitive.h"
#include "com/google/gson/jxx.com.google.gson.TypeAdapter.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace com::google::gson {
namespace {
std::string escapeString(const std::string& value, ::jxx::lang::jbool htmlSafe) {
    std::string result = "\"";
    const char* digits = "0123456789ABCDEF";
    for (unsigned char current : value) {
        switch (current) {
            case '"': result += "\\\""; break;
            case '\\': result += "\\\\"; break;
            case '\b': result += "\\b"; break;
            case '\f': result += "\\f"; break;
            case '\n': result += "\\n"; break;
            case '\r': result += "\\r"; break;
            case '\t': result += "\\t"; break;
            default:
                if (current < 0x20U ||
                    (htmlSafe && (current == '<' || current == '>' || current == '&' || current == '=' || current == '\''))) {
                    result += "\\u00";
                    result.push_back(digits[(current >> 4U) & 0x0FU]);
                    result.push_back(digits[current & 0x0FU]);
                } else {
                    result.push_back(static_cast<char>(current));
                }
        }
    }
    result.push_back('"');
    return result;
}

std::string indent(::jxx::lang::jint depth) {
    return std::string(static_cast<std::size_t>(depth * 2), ' ');
}
}

Gson::Gson(
    ::jxx::lang::jbool serializeNulls,
    ::jxx::lang::jbool htmlSafe,
    ::jxx::lang::jbool prettyPrinting,
    ::jxx::lang::jbool lenient)
    : serializeNulls_(serializeNulls), htmlSafe_(htmlSafe),
      prettyPrinting_(prettyPrinting), lenient_(lenient) {}

::jxx::Ptr<JsonElement> Gson::fromJson(
    const ::jxx::Ptr<::jxx::lang::String>& json) const {
    return JsonParser::parseString(json);
}

std::string Gson::formatTree_(
    const ::jxx::Ptr<JsonElement>& element,
    ::jxx::lang::jbool serializeNulls,
    ::jxx::lang::jbool htmlSafe,
    ::jxx::lang::jbool prettyPrinting,
    ::jxx::lang::jint depth) {
    if (element == nullptr || element->isJsonNull()) return "null";
    if (element->isJsonPrimitive()) {
        const auto primitive = element->getAsJsonPrimitive();
        return primitive->isString()
            ? escapeString(primitive->getAsString()->utf8(), htmlSafe)
            : primitive->toString()->utf8();
    }
    if (element->isJsonArray()) {
        const auto array = element->getAsJsonArray();
        if (array->size() == 0) return "[]";
        std::string output = "[";
        for (::jxx::lang::jint index = 0; index < array->size(); ++index) {
            if (index != 0) output += ",";
            if (prettyPrinting) output += "\n" + indent(depth + 1);
            output += formatTree_(array->get(index), serializeNulls, htmlSafe, prettyPrinting, depth + 1);
        }
        if (prettyPrinting) output += "\n" + indent(depth);
        return output + "]";
    }
    const auto object = element->getAsJsonObject();
    const auto entries = object->entrySet();
    const auto iterator = entries->iterator();
    std::string output = "{";
    ::jxx::lang::jbool first = true;
    while (iterator->hasNext()) {
        const auto entry = iterator->next();
        const auto value = entry->getValue();
        if (!serializeNulls && (value == nullptr || value->isJsonNull())) continue;
        if (!first) output += ",";
        if (prettyPrinting) output += "\n" + indent(depth + 1);
        output += escapeString(entry->getKey()->utf8(), htmlSafe);
        output += prettyPrinting ? ": " : ":";
        output += formatTree_(value, serializeNulls, htmlSafe, prettyPrinting, depth + 1);
        first = false;
    }
    if (prettyPrinting && !first) output += "\n" + indent(depth);
    return output + "}";
}

::jxx::Ptr<::jxx::lang::String> Gson::toJson(
    const ::jxx::Ptr<JsonElement>& element) const {
    return ::jxx::NEW<::jxx::lang::String>(
        formatTree_(element, serializeNulls_, htmlSafe_, prettyPrinting_, 0));
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

::jxx::lang::jbool Gson::serializeNulls() const noexcept { return serializeNulls_; }
::jxx::lang::jbool Gson::htmlSafe() const noexcept { return htmlSafe_; }
::jxx::lang::jbool Gson::prettyPrinting() const noexcept { return prettyPrinting_; }
::jxx::lang::jbool Gson::lenient() const noexcept { return lenient_; }

} // namespace com::google::gson
