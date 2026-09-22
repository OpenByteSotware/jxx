#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::lang { class String; }
namespace com::google::gson {
class JsonArray; class JsonObject; class JsonPrimitive;
class JsonElement : public ::jxx::lang::ClassBase<JsonElement, ::jxx::lang::Object> {
public:
    virtual ~JsonElement() = default;
    virtual ::jxx::lang::jbool isJsonNull() const noexcept;
    virtual ::jxx::lang::jbool isJsonPrimitive() const noexcept;
    virtual ::jxx::lang::jbool isJsonArray() const noexcept;
    virtual ::jxx::lang::jbool isJsonObject() const noexcept;
    virtual ::jxx::Ptr<JsonPrimitive> getAsJsonPrimitive();
    virtual ::jxx::Ptr<JsonArray> getAsJsonArray();
    virtual ::jxx::Ptr<JsonObject> getAsJsonObject();
    virtual ::jxx::Ptr<::jxx::lang::String> getAsString();
    virtual ::jxx::lang::jbool getAsBoolean();
    virtual ::jxx::lang::jint getAsInt();
    virtual ::jxx::lang::jlong getAsLong();
    virtual ::jxx::lang::jdouble getAsDouble();
};
} // namespace com::google::gson
