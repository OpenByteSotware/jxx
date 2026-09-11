#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"

namespace jxx::lang {
class String;
}

namespace jxx::org::w3c::dom {

class DOMStringList
    : public ::jxx::lang::InterfaceBase<DOMStringList> {
public:
    ~DOMStringList() override = default;

    virtual ::jxx::Ptr<::jxx::lang::String> item(
        ::jxx::lang::jint index) const = 0;

    virtual ::jxx::lang::jint getLength() const = 0;

    virtual ::jxx::lang::jbool contains(
        const ::jxx::Ptr<::jxx::lang::String>& value) const = 0;
};

} // namespace jxx::org::w3c::dom
