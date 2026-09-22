#pragma once
#include "ext/xml/namespace/jxx.ext.xml.namespace.QName.h"
namespace jxx::ext::xml::xpath {
class XPathConstants final {
public:
    static ::jxx::Ptr<::jxx::ext::xml::namespace_::QName> NUMBER();
    static ::jxx::Ptr<::jxx::ext::xml::namespace_::QName> STRING();
    static ::jxx::Ptr<::jxx::ext::xml::namespace_::QName> BOOLEAN();
    static ::jxx::Ptr<::jxx::ext::xml::namespace_::QName> NODESET();
    static ::jxx::Ptr<::jxx::ext::xml::namespace_::QName> NODE();
    static ::jxx::Ptr<::jxx::lang::String> DOM_OBJECT_MODEL();
};
} // namespace jxx::ext::xml::xpath
