#pragma once
#include "com/google/gson/jxx.com.google.gson.JsonElement.h"
#include "lang/jxx.lang.String.h"
namespace com::google::gson {
class JsonPrimitive final : public ::jxx::lang::ClassBase<JsonPrimitive, JsonElement> {
public:
    explicit JsonPrimitive(const ::jxx::Ptr<::jxx::lang::String>& value);
    explicit JsonPrimitive(::jxx::lang::jbool value);
    explicit JsonPrimitive(::jxx::lang::jlong value);
    explicit JsonPrimitive(::jxx::lang::jdouble value);
    ::jxx::lang::jbool isJsonPrimitive() const noexcept override { return true; }
    ::jxx::lang::jbool isString() const noexcept;
    ::jxx::lang::jbool isBoolean() const noexcept;
    ::jxx::lang::jbool isNumber() const noexcept;
    ::jxx::Ptr<::jxx::lang::String> getAsString() override;
    ::jxx::lang::jbool getAsBoolean() override;
    ::jxx::lang::jchar getAsCharacter() override;
    ::jxx::lang::jbyte getAsByte() override;
    ::jxx::lang::jshort getAsShort() override;
    ::jxx::lang::jint getAsInt() override;
    ::jxx::lang::jfloat getAsFloat() override;
    ::jxx::lang::jlong getAsLong() override;
    ::jxx::lang::jdouble getAsDouble() override;
    ::jxx::Ptr<::jxx::lang::String> toString() const override;
    ::jxx::lang::jbool equals(
        const ::jxx::Ptr<::jxx::lang::Object>& other) const override;
    ::jxx::lang::jint hashCode() const override;
private:
    enum class Kind { STRING, BOOLEAN, INTEGER, DECIMAL };
    Kind kind_; ::jxx::Ptr<::jxx::lang::String> value_;
};
} // namespace com::google::gson
