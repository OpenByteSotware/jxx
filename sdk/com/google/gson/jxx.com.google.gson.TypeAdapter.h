#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"

namespace com::google::gson {
class JsonElement;

class TypeAdapter
    : public ::jxx::lang::InterfaceBase<TypeAdapter> {
public:
    ~TypeAdapter() override = default;

    virtual ::jxx::Ptr<::jxx::lang::Object> fromJsonTree(
        const ::jxx::Ptr<JsonElement>& element) = 0;

    virtual ::jxx::Ptr<JsonElement> toJsonTree(
        const ::jxx::Ptr<::jxx::lang::Object>& value) = 0;

    static ::jxx::Ptr<TypeAdapter> nullSafe(
        const ::jxx::Ptr<TypeAdapter>& delegate);
};

} // namespace com::google::gson
