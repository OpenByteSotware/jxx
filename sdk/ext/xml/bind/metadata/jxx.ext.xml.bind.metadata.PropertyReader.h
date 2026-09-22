#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::ext::xml::bind::metadata {

class PropertyReader
    : public ::jxx::lang::InterfaceBase<PropertyReader> {
public:
    ~PropertyReader() override = default;

    virtual ::jxx::Ptr<::jxx::lang::Object> read(
        const ::jxx::Ptr<::jxx::lang::Object>& source) = 0;
};

} // namespace jxx::ext::xml::bind::metadata
