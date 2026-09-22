#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"

#include <string>

namespace jxx::lang { class String; }
namespace com::google::gson { class JsonElement; class TypeAdapter; }

namespace com::google::gson {

class Gson final
    : public ::jxx::lang::ClassBase<Gson, ::jxx::lang::Object> {
public:
    Gson() = default;
    Gson(
        ::jxx::lang::jbool serializeNulls,
        ::jxx::lang::jbool htmlSafe,
        ::jxx::lang::jbool prettyPrinting,
        ::jxx::lang::jbool lenient);

    ::jxx::Ptr<JsonElement> fromJson(
        const ::jxx::Ptr<::jxx::lang::String>& json) const;

    ::jxx::Ptr<::jxx::lang::String> toJson(
        const ::jxx::Ptr<JsonElement>& element) const;

    ::jxx::Ptr<::jxx::lang::Object> fromJson(
        const ::jxx::Ptr<::jxx::lang::String>& json,
        const ::jxx::Ptr<TypeAdapter>& adapter) const;

    ::jxx::Ptr<::jxx::lang::String> toJson(
        const ::jxx::Ptr<::jxx::lang::Object>& value,
        const ::jxx::Ptr<TypeAdapter>& adapter) const;

    ::jxx::lang::jbool serializeNulls() const noexcept;
    ::jxx::lang::jbool htmlSafe() const noexcept;
    ::jxx::lang::jbool prettyPrinting() const noexcept;
    ::jxx::lang::jbool lenient() const noexcept;

private:
    static std::string formatTree_(
        const ::jxx::Ptr<JsonElement>& element,
        ::jxx::lang::jbool serializeNulls,
        ::jxx::lang::jbool htmlSafe,
        ::jxx::lang::jbool prettyPrinting,
        ::jxx::lang::jint depth);

    ::jxx::lang::jbool serializeNulls_ = false;
    ::jxx::lang::jbool htmlSafe_ = true;
    ::jxx::lang::jbool prettyPrinting_ = false;
    ::jxx::lang::jbool lenient_ = false;
};

} // namespace com::google::gson
