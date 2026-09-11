#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Node.h"

namespace jxx::org::w3c::dom {

class Notation
    : public ::jxx::lang::InterfaceBase<
          Notation,
          Node> {
public:
    ~Notation() override = default;

    virtual ::jxx::Ptr<::jxx::lang::String>
    getPublicId() const = 0;

    virtual ::jxx::Ptr<::jxx::lang::String>
    getSystemId() const = 0;
};

} // namespace jxx::org::w3c::dom
