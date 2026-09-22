#pragma once

#include "lang/jxx.lang.Class.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"

namespace jxx::ext::xml::bind::annotation {

class XmlType final
    : public ::jxx::lang::ClassBase<XmlType, ::jxx::lang::Object> {
public:
    using StringArray = ::jxx::lang::JxxArray<
        ::jxx::Ptr<::jxx::lang::String>, 1U>;

    XmlType(
        const ::jxx::Ptr<::jxx::lang::String>& name = DEFAULT(),
        const ::jxx::Ptr<::jxx::lang::String>& nameSpace = DEFAULT(),
        const ::jxx::Ptr<StringArray>& propertyOrder = nullptr,
        const ::jxx::Ptr<::jxx::lang::ClassAny>& factoryClass = nullptr,
        const ::jxx::Ptr<::jxx::lang::String>& factoryMethod = DEFAULT());

    static ::jxx::Ptr<::jxx::lang::String> DEFAULT();
    ::jxx::Ptr<::jxx::lang::String> name() const;
    ::jxx::Ptr<::jxx::lang::String> nameSpace() const;
    ::jxx::Ptr<StringArray> propertyOrder() const;
    ::jxx::Ptr<::jxx::lang::ClassAny> factoryClass() const;
    ::jxx::Ptr<::jxx::lang::String> factoryMethod() const;

private:
    ::jxx::Ptr<::jxx::lang::String> name_;
    ::jxx::Ptr<::jxx::lang::String> namespace_;
    ::jxx::Ptr<StringArray> propertyOrder_;
    ::jxx::Ptr<::jxx::lang::ClassAny> factoryClass_;
    ::jxx::Ptr<::jxx::lang::String> factoryMethod_;
};

} // namespace jxx::ext::xml::bind::annotation
