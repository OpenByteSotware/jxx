#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"

namespace jxx::lang {

class String;

} // namespace jxx::lang

namespace jxx::org::w3c::dom {

class NameList
    : public ::jxx::lang::InterfaceBase<NameList> {
public:
    ~NameList() override = default;

    virtual ::jxx::Ptr<::jxx::lang::String>
    getName(
        ::jxx::lang::jint index) const = 0;

    virtual ::jxx::Ptr<::jxx::lang::String>
    getNamespaceURI(
        ::jxx::lang::jint index) const = 0;

    virtual ::jxx::lang::jint
    getLength() const = 0;

    virtual ::jxx::lang::jbool contains(
        const ::jxx::Ptr<::jxx::lang::String>& name) const = 0;

    virtual ::jxx::lang::jbool containsNS(
        const ::jxx::Ptr<::jxx::lang::String>& namespaceURI,
        const ::jxx::Ptr<::jxx::lang::String>& name) const = 0;
};

} // namespace jxx::org::w3c::dom
