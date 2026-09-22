#pragma once
#include "com/google/gson/jxx.com.google.gson.JsonElement.h"
#include "util/jxx.util.ArrayList.h"
namespace com::google::gson {
class JsonArray final : public ::jxx::lang::ClassBase<JsonArray, JsonElement> {
public:
    JsonArray();
    ::jxx::lang::jbool isJsonArray() const noexcept override { return true; }
    void add(const ::jxx::Ptr<JsonElement>& element);
    ::jxx::Ptr<JsonElement> get(::jxx::lang::jint index) const;
    void set(::jxx::lang::jint index, const ::jxx::Ptr<JsonElement>& element);
    ::jxx::Ptr<JsonElement> remove(::jxx::lang::jint index);
    ::jxx::lang::jbool remove(const ::jxx::Ptr<JsonElement>& element);
    ::jxx::lang::jbool contains(const ::jxx::Ptr<JsonElement>& element) const;
    ::jxx::lang::jint size() const;
    ::jxx::Ptr<JsonElement> deepCopy() override;
    ::jxx::Ptr<::jxx::lang::String> toString() const override;
private: ::jxx::Ptr<::jxx::util::ArrayList<JsonElement>> elements_;
};
} // namespace com::google::gson
