#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::ext::xml::bind::metadata {

class RepeatedPropertyWriter
    : public ::jxx::lang::InterfaceBase<RepeatedPropertyWriter> {
public:
    ~RepeatedPropertyWriter() override = default;

    virtual void add(
        const ::jxx::Ptr<::jxx::lang::Object>& target,
        const ::jxx::Ptr<::jxx::lang::Object>& value) = 0;
};

} // namespace jxx::ext::xml::bind::metadata
