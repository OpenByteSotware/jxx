#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::ext::xml::bind::annotation::adapters {

class XmlAdapter
    : public ::jxx::lang::InterfaceBase<XmlAdapter> {
public:
    ~XmlAdapter() override = default;

    virtual ::jxx::Ptr<::jxx::lang::Object> unmarshal(
        const ::jxx::Ptr<::jxx::lang::Object>& value) = 0;

    virtual ::jxx::Ptr<::jxx::lang::Object> marshal(
        const ::jxx::Ptr<::jxx::lang::Object>& bound) = 0;
};

} // namespace jxx::ext::xml::bind::annotation::adapters
