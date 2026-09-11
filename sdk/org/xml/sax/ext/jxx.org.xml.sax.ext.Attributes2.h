#pragma once

#include "org/xml/sax/jxx.org.xml.sax.Attributes.h"

namespace jxx::org::xml::sax::ext {

class Attributes2
    : public ::jxx::lang::InterfaceBase<
          Attributes2,
          ::jxx::org::xml::sax::Attributes> {
public:
    ~Attributes2() override = default;

    virtual ::jxx::lang::jbool isDeclared(
        ::jxx::lang::jint index) const = 0;

    virtual ::jxx::lang::jbool isDeclared(
        const ::jxx::Ptr<::jxx::lang::String>& qualifiedName) const = 0;

    virtual ::jxx::lang::jbool isDeclared(
        const ::jxx::Ptr<::jxx::lang::String>& uri,
        const ::jxx::Ptr<::jxx::lang::String>& localName) const = 0;

    virtual ::jxx::lang::jbool isSpecified(
        ::jxx::lang::jint index) const = 0;

    virtual ::jxx::lang::jbool isSpecified(
        const ::jxx::Ptr<::jxx::lang::String>& qualifiedName) const = 0;

    virtual ::jxx::lang::jbool isSpecified(
        const ::jxx::Ptr<::jxx::lang::String>& uri,
        const ::jxx::Ptr<::jxx::lang::String>& localName) const = 0;
};

} // namespace jxx::org::xml::sax::ext
