#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::ext::xml::bind::metadata {

class PropertyWriter
    : public ::jxx::lang::InterfaceBase<PropertyWriter> {
public:
    ~PropertyWriter() override = default;
    virtual void write(
        const ::jxx::Ptr<::jxx::lang::Object>& target,
        const ::jxx::Ptr<::jxx::lang::Object>& value) = 0;
};

} // namespace jxx::ext::xml::bind::metadata
