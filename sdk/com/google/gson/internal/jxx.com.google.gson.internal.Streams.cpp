#include "com/google/gson/internal/jxx.com.google.gson.internal.Streams.h"
#include "com/google/gson/jxx.com.google.gson.JsonArray.h"
#include "com/google/gson/jxx.com.google.gson.JsonNull.h"
#include "com/google/gson/jxx.com.google.gson.JsonObject.h"
#include "com/google/gson/jxx.com.google.gson.JsonPrimitive.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace com::google::gson::internal {
namespace {
using namespace ::com::google::gson;
using namespace ::com::google::gson::stream;

::jxx::Ptr<JsonElement> readValue(const ::jxx::Ptr<JsonReader>& reader) {
    switch (reader->peek()) {
        case JsonToken::BEGIN_ARRAY: {
            const auto array = ::jxx::NEW<JsonArray>();
            reader->beginArray();
            while (reader->hasNext()) array->add(readValue(reader));
            reader->endArray();
            return array;
        }
        case JsonToken::BEGIN_OBJECT: {
            const auto object = ::jxx::NEW<JsonObject>();
            reader->beginObject();
            while (reader->hasNext()) {
                const auto name = reader->nextName();
                object->add(name, readValue(reader));
            }
            reader->endObject();
            return object;
        }
        case JsonToken::STRING:
            return ::jxx::NEW<JsonPrimitive>(reader->nextString());
        case JsonToken::NUMBER: {
            const auto lexical = reader->nextString();
            const auto text = lexical->utf8();
            return text.find_first_of(".eE") == std::string::npos
                ? ::jxx::CAST<JsonElement>(::jxx::NEW<JsonPrimitive>(static_cast<::jxx::lang::jlong>(std::stoll(text))))
                : ::jxx::CAST<JsonElement>(::jxx::NEW<JsonPrimitive>(std::stod(text)));
        }
        case JsonToken::BOOLEAN:
            return ::jxx::NEW<JsonPrimitive>(reader->nextBoolean());
        case JsonToken::NULL_VALUE:
            reader->nextNull();
            return JsonNull::INSTANCE();
        default:
            return nullptr;
    }
}

void writeValue(const ::jxx::Ptr<JsonElement>& element, const ::jxx::Ptr<JsonWriter>& writer) {
    if (element == nullptr || element->isJsonNull()) { writer->nullValue(); return; }
    if (element->isJsonArray()) {
        const auto array = element->getAsJsonArray();
        writer->beginArray();
        for (::jxx::lang::jint index = 0; index < array->size(); ++index) writeValue(array->get(index), writer);
        writer->endArray();
        return;
    }
    if (element->isJsonObject()) {
        writer->beginObject();
        const auto iterator = element->getAsJsonObject()->entrySet()->iterator();
        while (iterator->hasNext()) {
            const auto entry = iterator->next();
            writer->name(entry->getKey());
            writeValue(entry->getValue(), writer);
        }
        writer->endObject();
        return;
    }
    const auto primitive = element->getAsJsonPrimitive();
    if (primitive->isBoolean()) writer->value(primitive->getAsBoolean());
    else if (primitive->isString()) writer->value(primitive->getAsString());
    else writer->jsonValue(primitive->getAsString());
}
}

::jxx::Ptr<::com::google::gson::JsonElement> Streams::parse(
    const ::jxx::Ptr<::com::google::gson::stream::JsonReader>& reader) {
    if (reader == nullptr) throw ::jxx::lang::NullPointerException();
    return readValue(reader);
}

void Streams::write(
    const ::jxx::Ptr<::com::google::gson::JsonElement>& element,
    const ::jxx::Ptr<::com::google::gson::stream::JsonWriter>& writer) {
    if (writer == nullptr) throw ::jxx::lang::NullPointerException();
    writeValue(element, writer);
}

} // namespace com::google::gson::internal
