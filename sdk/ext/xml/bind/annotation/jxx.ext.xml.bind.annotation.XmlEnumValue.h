#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
namespace jxx::ext::xml::bind::annotation {
class XmlEnumValue final
    : public ::jxx::lang::ClassBase<XmlEnumValue, ::jxx::lang::Object> {
public:
    explicit XmlEnumValue(const ::jxx::Ptr<::jxx::lang::String>& value);
    ::jxx::Ptr<::jxx::lang::String> value() const;
private:
    ::jxx::Ptr<::jxx::lang::String> value_;
};
} // namespace jxx::ext::xml::bind::annotation
