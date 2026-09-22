#pragma once
#include "com/google/gson/jxx.com.google.gson.JsonElement.h"
#include "com/google/gson/internal/jxx.com.google.gson.internal.LinkedTreeMap.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.Set.h"
#include "util/jxx.util.MapEntry.h"
#include "util/jxx.util.Collection.h"
namespace com::google::gson {
class JsonObject final : public ::jxx::lang::ClassBase<JsonObject, JsonElement> {
public:
    JsonObject();
    explicit JsonObject(const ::jxx::Ptr<JsonObject>& source);
    ::jxx::lang::jbool isJsonObject() const noexcept override { return true; }
    void add(const ::jxx::Ptr<::jxx::lang::String>& property,const ::jxx::Ptr<JsonElement>& value);
    void addProperty(const ::jxx::Ptr<::jxx::lang::String>& property,const ::jxx::Ptr<::jxx::lang::String>& value);
    void addProperty(const ::jxx::Ptr<::jxx::lang::String>& property,::jxx::lang::jbool value);
    void addProperty(const ::jxx::Ptr<::jxx::lang::String>& property,::jxx::lang::jchar value);
    void addProperty(const ::jxx::Ptr<::jxx::lang::String>& property,::jxx::lang::jlong value);
    void addProperty(const ::jxx::Ptr<::jxx::lang::String>& property,::jxx::lang::jdouble value);
    ::jxx::Ptr<JsonElement> get(const ::jxx::Ptr<::jxx::lang::String>& property) const;
    ::jxx::Ptr<JsonElement> remove(const ::jxx::Ptr<::jxx::lang::String>& property);
    ::jxx::Ptr<JsonArray> getAsJsonArray(const ::jxx::Ptr<::jxx::lang::String>& property) const;
    ::jxx::Ptr<JsonObject> getAsJsonObject(const ::jxx::Ptr<::jxx::lang::String>& property) const;
    ::jxx::Ptr<JsonPrimitive> getAsJsonPrimitive(const ::jxx::Ptr<::jxx::lang::String>& property) const;
    ::jxx::lang::jint size() const;
    ::jxx::lang::jbool isEmpty() const;
    ::jxx::lang::jbool has(const ::jxx::Ptr<::jxx::lang::String>& property) const;
    ::jxx::Ptr<::jxx::util::Set<
        ::jxx::util::MapEntry<::jxx::lang::String, JsonElement>>> entrySet() const;
    ::jxx::Ptr<::jxx::util::Set<::jxx::lang::String>> keySet() const;
    ::jxx::Ptr<::jxx::util::Collection<JsonElement>> values() const;
    ::jxx::Ptr<JsonElement> deepCopy() override;
    ::jxx::lang::jbool equals(
        const ::jxx::Ptr<::jxx::lang::Object>& other) const override;
    ::jxx::lang::jint hashCode() const override;
    ::jxx::Ptr<::jxx::lang::String> toString() const override;
private: ::jxx::Ptr<::com::google::gson::internal::LinkedTreeMap<::jxx::lang::String,JsonElement>> members_;
};
} // namespace com::google::gson
