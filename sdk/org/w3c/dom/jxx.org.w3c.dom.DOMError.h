#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"

namespace jxx::lang {
class Object;
class String;
}

namespace jxx::org::w3c::dom {

class DOMLocator;

class DOMError
    : public ::jxx::lang::InterfaceBase<DOMError> {
public:
    static constexpr ::jxx::lang::jshort SEVERITY_WARNING = 1;
    static constexpr ::jxx::lang::jshort SEVERITY_ERROR = 2;
    static constexpr ::jxx::lang::jshort SEVERITY_FATAL_ERROR = 3;

    ~DOMError() override = default;

    virtual ::jxx::lang::jshort getSeverity() const = 0;
    virtual ::jxx::Ptr<::jxx::lang::String> getMessage() const = 0;
    virtual ::jxx::Ptr<::jxx::lang::String> getType() const = 0;
    virtual ::jxx::Ptr<::jxx::lang::Object> getRelatedException() const = 0;
    virtual ::jxx::Ptr<::jxx::lang::Object> getRelatedData() const = 0;
    virtual ::jxx::Ptr<DOMLocator> getLocation() const = 0;
};

} // namespace jxx::org::w3c::dom
