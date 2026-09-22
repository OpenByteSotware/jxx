#include "com/google/gson/jxx.com.google.gson.Gson.h"

#include "com/google/gson/jxx.com.google.gson.JsonArray.h"
#include "com/google/gson/jxx.com.google.gson.JsonElement.h"
#include "com/google/gson/jxx.com.google.gson.JsonNull.h"
#include "com/google/gson/jxx.com.google.gson.JsonObject.h"
#include "com/google/gson/jxx.com.google.gson.JsonParser.h"
#include "com/google/gson/jxx.com.google.gson.JsonPrimitive.h"
#include "com/google/gson/jxx.com.google.gson.TypeAdapter.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "com/google/gson/internal/jxx.com.google.gson.internal.Streams.h"
#include "com/google/gson/stream/jxx.com.google.gson.stream.JsonReader.h"
#include "com/google/gson/stream/jxx.com.google.gson.stream.JsonWriter.h"
#include "io/jxx.io.Reader.h"
#include "io/jxx.io.Writer.h"

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
    ::jxx::lang::jbool lenient,
    const ::jxx::Ptr<FieldNamingStrategy>& fieldNamingStrategy,
    const ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::ClassAny>, 1U>>& adapterTypes,
    const ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<TypeAdapter>, 1U>>& adapters,
    const ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::ClassAny>, 1U>>& creatorTypes,
    const ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<InstanceCreator>, 1U>>& creators,
    const ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<TypeAdapterFactory>, 1U>>& factories,
    const ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<ExclusionStrategy>, 1U>>& serializationStrategies,
    const ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<ExclusionStrategy>, 1U>>& deserializationStrategies,
    ::jxx::lang::jbool requireExpose,
    ::jxx::lang::jdouble version,
    ::jxx::lang::jbool nonExecutableJson,
    ::jxx::lang::jbool specialFloatingPointValues,
    LongSerializationPolicy longSerializationPolicy,
    const ::jxx::Ptr<ToNumberStrategy>& objectToNumberStrategy,
    const ::jxx::Ptr<ToNumberStrategy>& numberToNumberStrategy,
    const ::jxx::Ptr<::jxx::lang::String>& dateFormatPattern,
    ::jxx::lang::jint dateStyle,
    ::jxx::lang::jbool serializeInnerClasses,
    ::jxx::lang::jbool complexMapKeySerialization)
    : serializeNulls_(serializeNulls), htmlSafe_(htmlSafe),
      prettyPrinting_(prettyPrinting), lenient_(lenient),
      fieldNamingStrategy_(fieldNamingStrategy), adapterTypes_(adapterTypes),
      adapters_(adapters), creatorTypes_(creatorTypes), creators_(creators),
      factories_(factories), serializationStrategies_(serializationStrategies),
      deserializationStrategies_(deserializationStrategies), requireExpose_(requireExpose),
      version_(version), nonExecutableJson_(nonExecutableJson),
      specialFloatingPointValues_(specialFloatingPointValues),
      longSerializationPolicy_(longSerializationPolicy),
      objectToNumberStrategy_(objectToNumberStrategy), numberToNumberStrategy_(numberToNumberStrategy),
      dateFormatPattern_(dateFormatPattern), dateStyle_(dateStyle),
      serializeInnerClasses_(serializeInnerClasses), complexMapKeySerialization_(complexMapKeySerialization) {}

::jxx::Ptr<JsonElement> Gson::fromJson(
    const ::jxx::Ptr<::jxx::lang::String>& json) const {
    return JsonParser::parseString(json);
}
::jxx::Ptr<JsonElement> Gson::fromJson(const ::jxx::Ptr<::jxx::io::Reader>& reader) const { if(reader==nullptr) throw ::jxx::lang::NullPointerException(); const auto stream=::jxx::NEW<::com::google::gson::stream::JsonReader>(reader); stream->setLenient(lenient_); return ::com::google::gson::internal::Streams::parse(stream); }
::jxx::Ptr<JsonElement> Gson::fromJson(const ::jxx::Ptr<::com::google::gson::stream::JsonReader>& reader) const { if(reader==nullptr) throw ::jxx::lang::NullPointerException(); return ::com::google::gson::internal::Streams::parse(reader); }

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
    auto text = formatTree_(element, serializeNulls_, htmlSafe_, prettyPrinting_, 0);
    if (nonExecutableJson_) text = ")]}'\n" + text;
    return ::jxx::NEW<::jxx::lang::String>(text);
}

void Gson::toJson(const ::jxx::Ptr<JsonElement>& element,const ::jxx::Ptr<::jxx::io::Writer>& writer) const { if(writer==nullptr) throw ::jxx::lang::NullPointerException(); writer->write(toJson(element)); }
void Gson::toJson(const ::jxx::Ptr<JsonElement>& element,const ::jxx::Ptr<::com::google::gson::stream::JsonWriter>& writer) const { if(writer==nullptr) throw ::jxx::lang::NullPointerException(); ::com::google::gson::internal::Streams::write(element,writer); }

::jxx::Ptr<::jxx::lang::Object> Gson::fromJson(
    const ::jxx::Ptr<::jxx::lang::String>& json,
    const ::jxx::Ptr<TypeAdapter>& adapter) const {
    if (adapter == nullptr) throw ::jxx::lang::NullPointerException();
    return adapter->fromJsonTree(JsonParser::parseString(json));
}

::jxx::Ptr<::jxx::lang::Object> Gson::fromJson(
    const ::jxx::Ptr<::jxx::lang::String>& json,
    const ::jxx::Ptr<::com::google::gson::reflect::TypeToken>& type) const {
    if (type == nullptr) throw ::jxx::lang::NullPointerException();
    const auto adapter = getAdapter(type);
    if (adapter == nullptr) return nullptr;
    return fromJson(json, adapter);
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
::jxx::Ptr<FieldNamingStrategy> Gson::fieldNamingStrategy() const { return fieldNamingStrategy_; }

::jxx::Ptr<TypeAdapter> Gson::getAdapter(
    const ::jxx::Ptr<::jxx::lang::ClassAny>& type) const {
    if (type == nullptr) throw ::jxx::lang::NullPointerException();
    if (adapterTypes_ != nullptr && adapters_ != nullptr) {
        for (::jxx::lang::jint index = adapters_->length - 1; index >= 0; --index) {
            if ((*adapterTypes_)[index] == type) return (*adapters_)[index];
        }
    }
    if (factories_ != nullptr) {
        for (::jxx::lang::jint index = factories_->length - 1; index >= 0; --index) {
            const auto adapter = (*factories_)[index]->create(type);
            if (adapter != nullptr) return adapter;
        }
    }
    return nullptr;
}

::jxx::Ptr<TypeAdapter> Gson::getAdapter(
    const ::jxx::Ptr<::com::google::gson::reflect::TypeToken>& type) const {
    if (type == nullptr) throw ::jxx::lang::NullPointerException();
    return getAdapter(type->getRawType());
}

::jxx::Ptr<::jxx::lang::Object> Gson::createInstance(
    const ::jxx::Ptr<::jxx::lang::ClassAny>& type) const {
    if (type == nullptr) throw ::jxx::lang::NullPointerException();
    if (creatorTypes_ != nullptr && creators_ != nullptr) {
        for (::jxx::lang::jint index = creators_->length - 1; index >= 0; --index) {
            if ((*creatorTypes_)[index] == type) return (*creators_)[index]->createInstance();
        }
    }
    return nullptr;
}

::jxx::lang::jbool Gson::shouldSkipField(
    const ::jxx::Ptr<FieldAttributes>& field,
    ::jxx::lang::jbool serialization) const {
    if (field == nullptr) throw ::jxx::lang::NullPointerException();
    const auto strategies = serialization
        ? serializationStrategies_
        : deserializationStrategies_;
    if (strategies != nullptr) {
        for (::jxx::lang::jint index = 0; index < strategies->length; ++index) {
            if ((*strategies)[index] != nullptr && (*strategies)[index]->shouldSkipField(field)) return true;
        }
    }
    return false;
}

::jxx::lang::jbool Gson::shouldSkipClass(
    const ::jxx::Ptr<::jxx::lang::ClassAny>& type,
    ::jxx::lang::jbool serialization) const {
    if (type == nullptr) throw ::jxx::lang::NullPointerException();
    const auto strategies = serialization
        ? serializationStrategies_
        : deserializationStrategies_;
    if (strategies != nullptr) {
        for (::jxx::lang::jint index = 0; index < strategies->length; ++index) {
            if ((*strategies)[index] != nullptr && (*strategies)[index]->shouldSkipClass(type)) return true;
        }
    }
    return false;
}

::jxx::lang::jbool Gson::requireExpose() const noexcept { return requireExpose_; }
::jxx::lang::jdouble Gson::version() const noexcept { return version_; }
LongSerializationPolicy Gson::longSerializationPolicy() const noexcept { return longSerializationPolicy_; }
::jxx::Ptr<ToNumberStrategy> Gson::objectToNumberStrategy() const { return objectToNumberStrategy_; }
::jxx::Ptr<ToNumberStrategy> Gson::numberToNumberStrategy() const { return numberToNumberStrategy_; }
::jxx::Ptr<::jxx::lang::String> Gson::dateFormatPattern() const { return dateFormatPattern_; }
::jxx::lang::jint Gson::dateStyle() const noexcept { return dateStyle_; }
::jxx::lang::jbool Gson::serializeInnerClasses() const noexcept { return serializeInnerClasses_; }
::jxx::lang::jbool Gson::complexMapKeySerialization() const noexcept { return complexMapKeySerialization_; }

} // namespace com::google::gson
