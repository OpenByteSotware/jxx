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
        const ::jxx::Ptr<::jxx::lang::String>& defaultValue = DEFAULT());

    static ::jxx::Ptr<::jxx::lang::String> DEFAULT();
    ::jxx::Ptr<::jxx::lang::String> name() const;
    ::jxx::lang::jbool required() const noexcept;
    ::jxx::lang::jbool nillable() const noexcept;
    ::jxx::Ptr<::jxx::lang::String> defaultValue() const;

private:
    ::jxx::Ptr<::jxx::lang::String> name_;
    ::jxx::lang::jbool required_;
    ::jxx::lang::jbool nillable_;
    ::jxx::Ptr<::jxx::lang::String> defaultValue_;
};

} // namespace jxx::ext::xml::bind::annotation
