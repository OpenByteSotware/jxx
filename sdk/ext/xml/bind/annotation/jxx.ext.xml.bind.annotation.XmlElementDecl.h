#pragma once

#include "lang/jxx.lang.Class.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

namespace jxx::ext::xml::bind::annotation {

class XmlElementDecl final
    : public ::jxx::lang::ClassBase<XmlElementDecl, ::jxx::lang::Object> {
public:
    XmlElementDecl(
        const ::jxx::Ptr<::jxx::lang::String>& name,
        const ::jxx::Ptr<::jxx::lang::String>& nameSpace = DEFAULT(),
        const ::jxx::Ptr<::jxx::lang::ClassAny>& scope = nullptr,
        const ::jxx::Ptr<::jxx::lang::String>& defaultValue = DEFAULT(),
        const ::jxx::Ptr<::jxx::lang::String>& substitutionHeadName = DEFAULT(),
        const ::jxx::Ptr<::jxx::lang::String>& substitutionHeadNamespace = DEFAULT());

    static ::jxx::Ptr<::jxx::lang::String> DEFAULT();
    ::jxx::Ptr<::jxx::lang::String> name() const;
    ::jxx::Ptr<::jxx::lang::String> nameSpace() const;
    ::jxx::Ptr<::jxx::lang::ClassAny> scope() const;
    ::jxx::Ptr<::jxx::lang::String> defaultValue() const;
    ::jxx::Ptr<::jxx::lang::String> substitutionHeadName() const;
    ::jxx::Ptr<::jxx::lang::String> substitutionHeadNamespace() const;

private:
    ::jxx::Ptr<::jxx::lang::String> name_;
    ::jxx::Ptr<::jxx::lang::String> namespace_;
    ::jxx::Ptr<::jxx::lang::ClassAny> scope_;
    ::jxx::Ptr<::jxx::lang::String> defaultValue_;
    ::jxx::Ptr<::jxx::lang::String> substitutionHeadName_;
    ::jxx::Ptr<::jxx::lang::String> substitutionHeadNamespace_;
};

} // namespace jxx::ext::xml::bind::annotation
