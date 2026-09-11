#pragma once

#include "org/xml/sax/jxx.org.xml.sax.XMLReader.h"

namespace jxx::org::xml::sax {

class XMLFilter
    : public ::jxx::lang::InterfaceBase<XMLFilter, XMLReader> {
public:
    ~XMLFilter() override = default;

    virtual void setParent(
        const ::jxx::Ptr<XMLReader>& parent) = 0;

    virtual ::jxx::Ptr<XMLReader> getParent() const = 0;
};

} // namespace jxx::org::xml::sax
