#pragma once

#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::org::w3c::dom {

class DOMError;

class DOMErrorHandler
    : public ::jxx::lang::InterfaceBase<DOMErrorHandler> {
public:
    ~DOMErrorHandler() override = default;

    virtual ::jxx::lang::jbool handleError(
        const ::jxx::Ptr<DOMError>& error) = 0;
};

} // namespace jxx::org::w3c::dom
