#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"

namespace jxx::lang {

class String;

} // namespace jxx::lang

namespace jxx::org::w3c::dom {

class DOMImplementation;
class DOMImplementationList;

class DOMImplementationSource
    : public ::jxx::lang::InterfaceBase<DOMImplementationSource> {
public:
    ~DOMImplementationSource() override = default;

    virtual ::jxx::Ptr<DOMImplementation> getDOMImplementation(
        const ::jxx::Ptr<::jxx::lang::String>& features) = 0;

    virtual ::jxx::Ptr<DOMImplementationList> getDOMImplementationList(
        const ::jxx::Ptr<::jxx::lang::String>& features) = 0;
};

} // namespace jxx::org::w3c::dom
