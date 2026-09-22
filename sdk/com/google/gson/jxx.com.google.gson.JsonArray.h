#pragma once
#include "com/google/gson/jxx.com.google.gson.JsonElement.h"
#include "util/jxx.util.ArrayList.h"
#include "util/jxx.util.Iterator.h"
namespace com::google::gson {
class JsonArray final : public ::jxx::lang::ClassBase<JsonArray, JsonElement> {
public:
    JsonArray();
    ::jxx::lang::jbool isJsonArray() const noexcept override { return true; }
    void add(const ::jxx::Ptr<JsonElement>& element);
    void add(const ::jxx::Ptr<::jxx::lang::String>& value);
    void add(::jxx::lang::jbool value);
    void add(::jxx::lang::jlong value);
    void add(::jxx::lang::jdouble value);
    void addAll(const ::jxx::Ptr<JsonArray>& array);
    ::jxx::Ptr<JsonElement> get(::jxx::lang::jint index) const;
    void set(::jxx::lang::jint index, const ::jxx::Ptr<JsonElement>& element);
    ::jxx::Ptr<JsonElement> remove(::jxx::lang::jint index);
    ::jxx::lang::jbool remove(const ::jxx::Ptr<JsonElement>& element);
    ::jxx::lang::jbool contains(const ::jxx::Ptr<JsonElement>& element) const;
    ::jxx::lang::jint size() const;
    ::jxx::lang::jbool isEmpty() const;
    ::jxx::Ptr<::jxx::util::Iterator<JsonElement>> iterator() const;
    ::jxx::Ptr<JsonElement> deepCopy() override;
    ::jxx::Ptr<::jxx::lang::String> getAsString() override;
    ::jxx::lang::jbool getAsBoolean() override;
    ::jxx::lang::jbyte getAsByte() override;
    ::jxx::lang::jshort getAsShort() override;
    ::jxx::lang::jint getAsInt() override;
    ::jxx::lang::jfloat getAsFloat() override;
    ::jxx::lang::jlong getAsLong() override;
    ::jxx::lang::jdouble getAsDouble() override;
    ::jxx::lang::jbool equals(
        const ::jxx::Ptr<::jxx::lang::Object>& other) const override;
    ::jxx::lang::jint hashCode() const override;
    ::jxx::Ptr<::jxx::lang::String> toString() const override;
private: ::jxx::Ptr<::jxx::util::ArrayList<JsonElement>> elements_;
};
} // namespace com::google::gson
