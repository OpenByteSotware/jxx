#pragma once

#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"

namespace jxx::ext::xml::bind::annotation {

class XmlType final
    : public ::jxx::lang::ClassBase<XmlType, ::jxx::lang::Object> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<XmlType, JxxSuper>;
    using StringArray = ::jxx::JxxArray<::jxx::Ptr<::jxx::lang::String>, 1U>;

    XmlType(
        const ::jxx::Ptr<::jxx::lang::String>& name = DEFAULT(),
        const ::jxx::Ptr<::jxx::lang::String>& nameSpace = DEFAULT(),
        const ::jxx::Ptr<StringArray>& propertyOrder = nullptr)
        : Super(), name_(name), namespace_(nameSpace),
          propertyOrder_(propertyOrder) {}

    static ::jxx::Ptr<::jxx::lang::String> DEFAULT() {
        return ::jxx::NEW<::jxx::lang::String>("##default");
    }
    ::jxx::Ptr<::jxx::lang::String> name() const { return name_; }
    ::jxx::Ptr<::jxx::lang::String> nameSpace() const { return namespace_; }
    ::jxx::Ptr<StringArray> propertyOrder() const { return propertyOrder_; }

private:
    ::jxx::Ptr<::jxx::lang::String> name_;
    ::jxx::Ptr<::jxx::lang::String> namespace_;
    ::jxx::Ptr<StringArray> propertyOrder_;
};

} // namespace jxx::ext::xml::bind::annotation
