#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "ext/xml/namespace/jxx.ext.xml.namespace.QName.h"
namespace jxx::ext::xml::xpath {
class XPathConstants final
    : public ::jxx::lang::ClassBase<XPathConstants, ::jxx::lang::Object> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<XPathConstants, JxxSuper>;
    static ::jxx::Ptr<::jxx::ext::xml::namespace_::QName> NUMBER();
    static ::jxx::Ptr<::jxx::ext::xml::namespace_::QName> STRING();
    static ::jxx::Ptr<::jxx::ext::xml::namespace_::QName> BOOLEAN();
    static ::jxx::Ptr<::jxx::ext::xml::namespace_::QName> NODESET();
    static ::jxx::Ptr<::jxx::ext::xml::namespace_::QName> NODE();
    static ::jxx::Ptr<::jxx::lang::String> DOM_OBJECT_MODEL();
};
} // namespace jxx::ext::xml::xpath
