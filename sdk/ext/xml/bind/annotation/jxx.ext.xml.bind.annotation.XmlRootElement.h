#pragma once

#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.String.h"

namespace jxx::ext::xml::bind::annotation {

class XmlRootElement final
    : public ::jxx::lang::ClassBase<XmlRootElement, ::jxx::lang::Object> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<XmlRootElement, JxxSuper>;

    XmlRootElement(
        const ::jxx::Ptr<::jxx::lang::String>& name = DEFAULT(),
        const ::jxx::Ptr<::jxx::lang::String>& nameSpace = DEFAULT())
        : Super(), name_(name), namespace_(nameSpace) {}

    static ::jxx::Ptr<::jxx::lang::String> DEFAULT() {
        return ::jxx::NEW<::jxx::lang::String>("##default");
    }
    ::jxx::Ptr<::jxx::lang::String> name() const { return name_; }
    ::jxx::Ptr<::jxx::lang::String> nameSpace() const { return namespace_; }

private:
    ::jxx::Ptr<::jxx::lang::String> name_;
    ::jxx::Ptr<::jxx::lang::String> namespace_;
};

} // namespace jxx::ext::xml::bind::annotation
