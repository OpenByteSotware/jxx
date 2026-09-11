#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.buildin_array.h"
#include "lang/jxx_types.h"

namespace jxx::lang {
class String;
}

namespace jxx::org::xml::sax {

class Attributes;
class Locator;

class ContentHandler
    : public ::jxx::lang::InterfaceBase<ContentHandler> {
public:
    ~ContentHandler() override = default;

    virtual void setDocumentLocator(
        const ::jxx::Ptr<Locator>& locator) = 0;

    virtual void startDocument() = 0;
    virtual void endDocument() = 0;

    virtual void startPrefixMapping(
        const ::jxx::Ptr<::jxx::lang::String>& prefix,
        const ::jxx::Ptr<::jxx::lang::String>& uri) = 0;

    virtual void endPrefixMapping(
        const ::jxx::Ptr<::jxx::lang::String>& prefix) = 0;

    virtual void startElement(
        const ::jxx::Ptr<::jxx::lang::String>& uri,
        const ::jxx::Ptr<::jxx::lang::String>& localName,
        const ::jxx::Ptr<::jxx::lang::String>& qName,
        const ::jxx::Ptr<Attributes>& attributes) = 0;

    virtual void endElement(
        const ::jxx::Ptr<::jxx::lang::String>& uri,
        const ::jxx::Ptr<::jxx::lang::String>& localName,
        const ::jxx::Ptr<::jxx::lang::String>& qName) = 0;

    virtual void characters(
        const ::jxx::lang::CharArray& characters,
        ::jxx::lang::jint start,
        ::jxx::lang::jint length) = 0;

    virtual void ignorableWhitespace(
        const ::jxx::lang::CharArray& characters,
        ::jxx::lang::jint start,
        ::jxx::lang::jint length) = 0;

    virtual void processingInstruction(
        const ::jxx::Ptr<::jxx::lang::String>& target,
        const ::jxx::Ptr<::jxx::lang::String>& data) = 0;

    virtual void skippedEntity(
        const ::jxx::Ptr<::jxx::lang::String>& name) = 0;
};

} // namespace jxx::org::xml::sax
