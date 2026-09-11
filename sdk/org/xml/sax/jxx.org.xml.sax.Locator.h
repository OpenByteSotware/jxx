#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"

namespace jxx::lang {
class String;
}

namespace jxx::org::xml::sax {

class Locator
    : public ::jxx::lang::InterfaceBase<Locator> {
public:
    ~Locator() override = default;

    virtual ::jxx::Ptr<::jxx::lang::String>
    getPublicId() const = 0;

    virtual ::jxx::Ptr<::jxx::lang::String>
    getSystemId() const = 0;

    virtual ::jxx::lang::jint
    getLineNumber() const = 0;

    virtual ::jxx::lang::jint
    getColumnNumber() const = 0;
};

} // namespace jxx::org::xml::sax
