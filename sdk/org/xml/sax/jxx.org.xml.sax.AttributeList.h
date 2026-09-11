#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"

namespace jxx::lang {
class String;
}

namespace jxx::org::xml::sax {

class AttributeList
    : public ::jxx::lang::InterfaceBase<AttributeList> {
public:
    ~AttributeList() override = default;

    virtual ::jxx::lang::jint getLength() const = 0;
    virtual ::jxx::Ptr<::jxx::lang::String> getName(::jxx::lang::jint index) const = 0;
    virtual ::jxx::Ptr<::jxx::lang::String> getType(::jxx::lang::jint index) const = 0;
    virtual ::jxx::Ptr<::jxx::lang::String> getValue(::jxx::lang::jint index) const = 0;
    virtual ::jxx::Ptr<::jxx::lang::String> getType(
        const ::jxx::Ptr<::jxx::lang::String>& name) const = 0;
    virtual ::jxx::Ptr<::jxx::lang::String> getValue(
        const ::jxx::Ptr<::jxx::lang::String>& name) const = 0;
};

} // namespace jxx::org::xml::sax
