#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
namespace jxx::ext::xml::bind::annotation {
class XmlElementWrapper final
    : public ::jxx::lang::ClassBase<XmlElementWrapper, ::jxx::lang::Object> {
public:
    XmlElementWrapper(
        const ::jxx::Ptr<::jxx::lang::String>& name = DEFAULT(),
        const ::jxx::Ptr<::jxx::lang::String>& nameSpace = DEFAULT(),
        ::jxx::lang::jbool nillable = false,
        ::jxx::lang::jbool required = false);
    static ::jxx::Ptr<::jxx::lang::String> DEFAULT();
    ::jxx::Ptr<::jxx::lang::String> name() const;
    ::jxx::Ptr<::jxx::lang::String> nameSpace() const;
    ::jxx::lang::jbool nillable() const noexcept;
    ::jxx::lang::jbool required() const noexcept;
private:
    ::jxx::Ptr<::jxx::lang::String> name_;
    ::jxx::Ptr<::jxx::lang::String> namespace_;
    ::jxx::lang::jbool nillable_;
    ::jxx::lang::jbool required_;
};
} // namespace jxx::ext::xml::bind::annotation
