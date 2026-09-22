#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
namespace jxx::ext::xml::bind::annotation {
class XmlEnumValue final
    : public ::jxx::lang::ClassBase<XmlEnumValue, ::jxx::lang::Object> {
public:
    explicit XmlEnumValue(const ::jxx::Ptr<::jxx::lang::String>& value)
        : value_(value) {}
    ::jxx::Ptr<::jxx::lang::String> value() const { return value_; }
private:
    ::jxx::Ptr<::jxx::lang::String> value_;
};
} // namespace jxx::ext::xml::bind::annotation
