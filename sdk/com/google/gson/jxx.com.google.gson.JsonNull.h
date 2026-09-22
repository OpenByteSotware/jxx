#pragma once
#include "com/google/gson/jxx.com.google.gson.JsonElement.h"
namespace com::google::gson {
class JsonNull final : public ::jxx::lang::ClassBase<JsonNull, JsonElement> {
public:
    static ::jxx::Ptr<JsonNull> INSTANCE();
    ::jxx::lang::jbool isJsonNull() const noexcept override { return true; }
    ::jxx::Ptr<JsonElement> deepCopy() override;
    ::jxx::lang::jbool equals(
        const ::jxx::Ptr<::jxx::lang::Object>& other) const override;
    ::jxx::lang::jint hashCode() const override;
    ::jxx::Ptr<::jxx::lang::String> toString() const override;
};
} // namespace com::google::gson
