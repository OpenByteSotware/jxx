#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

namespace jxx::ext::xml::bind::metadata {

class ValueConverter
    : public ::jxx::lang::InterfaceBase<ValueConverter> {
public:
    ~ValueConverter() override = default;
    virtual ::jxx::Ptr<::jxx::lang::Object> convert(
        const ::jxx::Ptr<::jxx::lang::String>& text) = 0;
};

} // namespace jxx::ext::xml::bind::metadata
