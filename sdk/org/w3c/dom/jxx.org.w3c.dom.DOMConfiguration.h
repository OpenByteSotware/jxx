#pragma once

#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::lang {
class Object;
class String;
}

namespace jxx::org::w3c::dom {

class DOMStringList;

class DOMConfiguration
    : public ::jxx::lang::InterfaceBase<DOMConfiguration> {
public:
    ~DOMConfiguration() override = default;

    virtual void setParameter(
        const ::jxx::Ptr<::jxx::lang::String>& name,
        const ::jxx::Ptr<::jxx::lang::Object>& value) = 0;

    virtual ::jxx::Ptr<::jxx::lang::Object> getParameter(
        const ::jxx::Ptr<::jxx::lang::String>& name) const = 0;

    virtual ::jxx::lang::jbool canSetParameter(
        const ::jxx::Ptr<::jxx::lang::String>& name,
        const ::jxx::Ptr<::jxx::lang::Object>& value) const = 0;

    virtual ::jxx::Ptr<DOMStringList> getParameterNames() const = 0;
};

} // namespace jxx::org::w3c::dom
