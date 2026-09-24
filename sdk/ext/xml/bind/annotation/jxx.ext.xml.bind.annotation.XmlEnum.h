#pragma once
#include "lang/jxx.lang.Class.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
namespace jxx::ext::xml::bind::annotation {
class XmlEnum final
    : public ::jxx::lang::ClassBase<XmlEnum, ::jxx::lang::Object> {
public:
    explicit XmlEnum(const ::jxx::Ptr<::jxx::lang::ClassAny>& value = nullptr);
    ::jxx::Ptr<::jxx::lang::ClassAny> value() const;
private:
    ::jxx::Ptr<::jxx::lang::ClassAny> value_;
};
} // namespace jxx::ext::xml::bind::annotation
