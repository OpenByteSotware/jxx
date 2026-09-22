#pragma once

#include "ext/xml/bind/jxx.ext.xml.bind.JAXBElementI.h"
#include "ext/xml/namespace/jxx.ext.xml.namespace.QName.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::ext::xml::bind::metadata {

class ElementWrapperFactory
    : public ::jxx::lang::InterfaceBase<ElementWrapperFactory> {
public:
    ~ElementWrapperFactory() override = default;

    virtual ::jxx::Ptr<::jxx::ext::xml::bind::JAXBElementI> wrap(
        const ::jxx::Ptr<::jxx::ext::xml::namespace_::QName>& name,
        const ::jxx::Ptr<::jxx::lang::Object>& value,
        ::jxx::lang::jbool nil) = 0;
};

} // namespace jxx::ext::xml::bind::metadata
