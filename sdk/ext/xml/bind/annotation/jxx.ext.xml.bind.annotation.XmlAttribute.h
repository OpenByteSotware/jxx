#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
namespace jxx::ext::xml::bind::annotation {
class XmlAttribute final
    : public ::jxx::lang::ClassBase<XmlAttribute, ::jxx::lang::Object> {
public:
    XmlAttribute(
        const ::jxx::Ptr<::jxx::lang::String>& name = DEFAULT(),
        const ::jxx::Ptr<::jxx::lang::String>& nameSpace = DEFAULT(),
        ::jxx::lang::jbool required = false);
    static ::jxx::Ptr<::jxx::lang::String> DEFAULT();
    ::jxx::Ptr<::jxx::lang::String> name() const;
    ::jxx::Ptr<::jxx::lang::String> nameSpace() const;
    ::jxx::lang::jbool required() const noexcept;
private:
    ::jxx::Ptr<::jxx::lang::String> name_;
    ::jxx::Ptr<::jxx::lang::String> namespace_;
    ::jxx::lang::jbool required_;
};
} // namespace jxx::ext::xml::bind::annotation
