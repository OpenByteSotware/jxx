#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"

namespace jxx::org::w3c::dom {

class DOMImplementation;

class DOMImplementationList
    : public ::jxx::lang::InterfaceBase<DOMImplementationList> {
public:
    ~DOMImplementationList() override = default;

    virtual ::jxx::Ptr<DOMImplementation> item(
        ::jxx::lang::jint index) const = 0;

    virtual ::jxx::lang::jint getLength() const = 0;
};

} // namespace jxx::org::w3c::dom
