#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"

namespace jxx::lang {
class String;
}

namespace jxx::org::xml::sax {

class Attributes
    : public ::jxx::lang::InterfaceBase<Attributes> {
public:
    ~Attributes() override = default;

    virtual ::jxx::lang::jint getLength() const = 0;

    virtual ::jxx::Ptr<::jxx::lang::String> getURI(
        ::jxx::lang::jint index) const = 0;

    virtual ::jxx::Ptr<::jxx::lang::String> getLocalName(
        ::jxx::lang::jint index) const = 0;

    virtual ::jxx::Ptr<::jxx::lang::String> getQName(
        ::jxx::lang::jint index) const = 0;

    virtual ::jxx::Ptr<::jxx::lang::String> getType(
        ::jxx::lang::jint index) const = 0;

    virtual ::jxx::Ptr<::jxx::lang::String> getValue(
        ::jxx::lang::jint index) const = 0;

    virtual ::jxx::lang::jint getIndex(
        const ::jxx::Ptr<::jxx::lang::String>& uri,
        const ::jxx::Ptr<::jxx::lang::String>& localName) const = 0;

    virtual ::jxx::lang::jint getIndex(
        const ::jxx::Ptr<::jxx::lang::String>& qName) const = 0;

    virtual ::jxx::Ptr<::jxx::lang::String> getType(
        const ::jxx::Ptr<::jxx::lang::String>& uri,
        const ::jxx::Ptr<::jxx::lang::String>& localName) const = 0;

    virtual ::jxx::Ptr<::jxx::lang::String> getType(
        const ::jxx::Ptr<::jxx::lang::String>& qName) const = 0;

    virtual ::jxx::Ptr<::jxx::lang::String> getValue(
        const ::jxx::Ptr<::jxx::lang::String>& uri,
        const ::jxx::Ptr<::jxx::lang::String>& localName) const = 0;

    virtual ::jxx::Ptr<::jxx::lang::String> getValue(
        const ::jxx::Ptr<::jxx::lang::String>& qName) const = 0;
};

} // namespace jxx::org::xml::sax
