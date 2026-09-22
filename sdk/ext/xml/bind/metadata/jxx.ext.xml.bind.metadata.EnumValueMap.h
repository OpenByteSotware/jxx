#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

namespace jxx::ext::xml::bind::metadata {

class EnumValueMap
    : public ::jxx::lang::InterfaceBase<EnumValueMap> {
public:
    ~EnumValueMap() override = default;

    virtual ::jxx::Ptr<::jxx::lang::Object> fromValue(
        const ::jxx::Ptr<::jxx::lang::String>& lexicalValue) = 0;

    virtual ::jxx::Ptr<::jxx::lang::String> value(
        const ::jxx::Ptr<::jxx::lang::Object>& enumValue) = 0;
};

} // namespace jxx::ext::xml::bind::metadata
