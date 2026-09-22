#pragma once

#include "ext/xml/bind/annotation/adapters/jxx.ext.xml.bind.annotation.adapters.XmlAdapter.h"
#include "lang/jxx.lang.Class.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::ext::xml::bind::annotation::adapters {

class XmlJavaTypeAdapter final
    : public ::jxx::lang::ClassBase<
          XmlJavaTypeAdapter,
          ::jxx::lang::Object> {
public:
    XmlJavaTypeAdapter(
        const ::jxx::Ptr<::jxx::lang::ClassAny>& value,
        const ::jxx::Ptr<::jxx::lang::ClassAny>& type = nullptr)
        : value_(value), type_(type) {}

    ::jxx::Ptr<::jxx::lang::ClassAny> value() const { return value_; }
    ::jxx::Ptr<::jxx::lang::ClassAny> type() const { return type_; }

private:
    ::jxx::Ptr<::jxx::lang::ClassAny> value_;
    ::jxx::Ptr<::jxx::lang::ClassAny> type_;
};

} // namespace jxx::ext::xml::bind::annotation::adapters
