#pragma once
#include "ext/xml/bind/annotation/jxx.ext.xml.bind.annotation.XmlElement.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.buildin_array.h"
namespace jxx::ext::xml::bind::annotation {
class XmlElements final
    : public ::jxx::lang::ClassBase<XmlElements, ::jxx::lang::Object> {
public:
    using ElementArray = ::jxx::lang::JxxArray<::jxx::Ptr<XmlElement>, 1U>;
    explicit XmlElements(const ::jxx::Ptr<ElementArray>& value) : value_(value) {}
    ::jxx::Ptr<ElementArray> value() const { return value_; }
private:
    ::jxx::Ptr<ElementArray> value_;
};
} // namespace jxx::ext::xml::bind::annotation
