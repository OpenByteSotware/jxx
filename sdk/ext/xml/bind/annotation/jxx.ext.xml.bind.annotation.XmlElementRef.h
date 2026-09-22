#pragma once
#include "lang/jxx.lang.Class.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
namespace jxx::ext::xml::bind::annotation {
class XmlElementRef final
    : public ::jxx::lang::ClassBase<XmlElementRef, ::jxx::lang::Object> {
public:
    XmlElementRef(
        const ::jxx::Ptr<::jxx::lang::String>& name = DEFAULT(),
        const ::jxx::Ptr<::jxx::lang::ClassAny>& type = nullptr,
        const ::jxx::Ptr<::jxx::lang::String>& nameSpace = DEFAULT(),
        ::jxx::lang::jbool required = true);
    static ::jxx::Ptr<::jxx::lang::String> DEFAULT();
    ::jxx::Ptr<::jxx::lang::String> name() const;
    ::jxx::Ptr<::jxx::lang::ClassAny> type() const;
    ::jxx::Ptr<::jxx::lang::String> nameSpace() const;
    ::jxx::lang::jbool required() const noexcept;
private:
    ::jxx::Ptr<::jxx::lang::String> name_;
    ::jxx::Ptr<::jxx::lang::ClassAny> type_;
    ::jxx::Ptr<::jxx::lang::String> namespace_;
    ::jxx::lang::jbool required_;
};
} // namespace jxx::ext::xml::bind::annotation
