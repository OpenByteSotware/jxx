#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::lang { class String; }

namespace jxx::ext::xml::bind::metadata {

class ValueFormatter
    : public ::jxx::lang::InterfaceBase<ValueFormatter> {
public:
    ~ValueFormatter() override = default;

    virtual ::jxx::Ptr<::jxx::lang::String> format(
        const ::jxx::Ptr<::jxx::lang::Object>& value) = 0;
};

} // namespace jxx::ext::xml::bind::metadata
