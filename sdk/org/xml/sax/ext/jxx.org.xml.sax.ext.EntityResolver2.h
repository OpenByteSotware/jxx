#pragma once

#include "org/xml/sax/jxx.org.xml.sax.EntityResolver.h"

namespace jxx::org::xml::sax::ext {

class EntityResolver2
    : public ::jxx::lang::InterfaceBase<
          EntityResolver2,
          ::jxx::org::xml::sax::EntityResolver> {
public:
    ~EntityResolver2() override = default;

    virtual ::jxx::Ptr<::jxx::org::xml::sax::InputSource>
    getExternalSubset(
        const ::jxx::Ptr<::jxx::lang::String>& name,
        const ::jxx::Ptr<::jxx::lang::String>& baseURI) = 0;

    virtual ::jxx::Ptr<::jxx::org::xml::sax::InputSource>
    resolveEntity(
        const ::jxx::Ptr<::jxx::lang::String>& name,
        const ::jxx::Ptr<::jxx::lang::String>& publicId,
        const ::jxx::Ptr<::jxx::lang::String>& baseURI,
        const ::jxx::Ptr<::jxx::lang::String>& systemId) = 0;
};

} // namespace jxx::org::xml::sax::ext
