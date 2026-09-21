#pragma once

#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.String.h"

namespace jxx::ext::xml::bind::annotation {

class XmlElement final
    : public ::jxx::lang::ClassBase<XmlElement, ::jxx::lang::Object> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<XmlElement, JxxSuper>;

    XmlElement(
        const ::jxx::Ptr<::jxx::lang::String>& name = DEFAULT(),
        ::jxx::lang::jbool required = false,
        ::jxx::lang::jbool nillable = false,
        const ::jxx::Ptr<::jxx::lang::String>& defaultValue = DEFAULT())
        : Super(), name_(name), required_(required), nillable_(nillable),
          defaultValue_(defaultValue) {}

    static ::jxx::Ptr<::jxx::lang::String> DEFAULT() {
        return ::jxx::NEW<::jxx::lang::String>("##default");
    }
    ::jxx::Ptr<::jxx::lang::String> name() const { return name_; }
    ::jxx::lang::jbool required() const noexcept { return required_; }
    ::jxx::lang::jbool nillable() const noexcept { return nillable_; }
    ::jxx::Ptr<::jxx::lang::String> defaultValue() const { return defaultValue_; }

private:
    ::jxx::Ptr<::jxx::lang::String> name_;
    ::jxx::lang::jbool required_;
    ::jxx::lang::jbool nillable_;
    ::jxx::Ptr<::jxx::lang::String> defaultValue_;
};

} // namespace jxx::ext::xml::bind::annotation
