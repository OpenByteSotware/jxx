#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

namespace jxx::ext::xml::bind::annotation {

class XmlRootElement final
    : public ::jxx::lang::ClassBase<XmlRootElement, ::jxx::lang::Object> {
public:
    XmlRootElement(
        const ::jxx::Ptr<::jxx::lang::String>& name = DEFAULT(),
        const ::jxx::Ptr<::jxx::lang::String>& nameSpace = DEFAULT());

    static ::jxx::Ptr<::jxx::lang::String> DEFAULT();
    ::jxx::Ptr<::jxx::lang::String> name() const;
    ::jxx::Ptr<::jxx::lang::String> nameSpace() const;

private:
    ::jxx::Ptr<::jxx::lang::String> name_;
    ::jxx::Ptr<::jxx::lang::String> namespace_;
};

} // namespace jxx::ext::xml::bind::annotation
