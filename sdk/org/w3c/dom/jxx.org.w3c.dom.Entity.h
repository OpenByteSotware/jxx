#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Node.h"

namespace jxx::org::w3c::dom {

class Entity
    : public ::jxx::lang::InterfaceBase<
          Entity,
          Node> {
public:
    ~Entity() override = default;

    virtual ::jxx::Ptr<::jxx::lang::String>
    getPublicId() const = 0;

    virtual ::jxx::Ptr<::jxx::lang::String>
    getSystemId() const = 0;

    virtual ::jxx::Ptr<::jxx::lang::String>
    getNotationName() const = 0;

    virtual ::jxx::Ptr<::jxx::lang::String>
    getInputEncoding() const = 0;

    virtual ::jxx::Ptr<::jxx::lang::String>
    getXmlEncoding() const = 0;

    virtual ::jxx::Ptr<::jxx::lang::String>
    getXmlVersion() const = 0;
};

} // namespace jxx::org::w3c::dom
