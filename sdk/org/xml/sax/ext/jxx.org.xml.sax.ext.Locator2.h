#pragma once

#include "org/xml/sax/jxx.org.xml.sax.Locator.h"

namespace jxx::org::xml::sax::ext {

class Locator2
    : public ::jxx::lang::InterfaceBase<
          Locator2,
          ::jxx::org::xml::sax::Locator> {
public:
    ~Locator2() override = default;

    virtual ::jxx::Ptr<::jxx::lang::String>
    getXMLVersion() const = 0;

    virtual ::jxx::Ptr<::jxx::lang::String>
    getEncoding() const = 0;
};

} // namespace jxx::org::xml::sax::ext
