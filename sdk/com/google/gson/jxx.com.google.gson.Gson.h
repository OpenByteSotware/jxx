#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::lang { class String; }
namespace com::google::gson { class JsonElement; }

namespace com::google::gson {

class Gson final
    : public ::jxx::lang::ClassBase<Gson, ::jxx::lang::Object> {
public:
    Gson() = default;

    ::jxx::Ptr<JsonElement> fromJson(
        const ::jxx::Ptr<::jxx::lang::String>& json) const;

    ::jxx::Ptr<::jxx::lang::String> toJson(
        const ::jxx::Ptr<JsonElement>& element) const;
};

} // namespace com::google::gson
