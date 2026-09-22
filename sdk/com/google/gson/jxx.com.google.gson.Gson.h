#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::lang { class String; }
namespace com::google::gson { class JsonElement; class TypeAdapter; }

namespace com::google::gson {

class Gson final
    : public ::jxx::lang::ClassBase<Gson, ::jxx::lang::Object> {
public:
    Gson() = default;

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
};

} // namespace com::google::gson
