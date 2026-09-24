#pragma once

#include "ext/xml/bind/annotation/jxx.ext.xml.bind.annotation.XmlAccessType.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::ext::xml::bind::annotation {

class XmlAccessorType final
    : public ::jxx::lang::ClassBase<XmlAccessorType, ::jxx::lang::Object> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<XmlAccessorType, JxxSuper>;

    explicit XmlAccessorType(XmlAccessType value);
    XmlAccessType value() const noexcept;

private:
    XmlAccessType value_;
};

} // namespace jxx::ext::xml::bind::annotation
