#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "org/xml/sax/jxx.org.xml.sax.ContentHandler.h"
#include "org/xml/sax/jxx.org.xml.sax.DTDHandler.h"
#include "org/xml/sax/jxx.org.xml.sax.EntityResolver.h"
#include "org/xml/sax/jxx.org.xml.sax.ErrorHandler.h"

namespace jxx::org::xml::sax::helpers {

class DefaultHandler
    : public ::jxx::lang::ClassBase<
          DefaultHandler,
          ::jxx::lang::Object,
          ::jxx::org::xml::sax::EntityResolver,
          ::jxx::org::xml::sax::DTDHandler,
          ::jxx::org::xml::sax::ContentHandler,
          ::jxx::org::xml::sax::ErrorHandler> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<
        DefaultHandler,
        JxxSuper,
        ::jxx::org::xml::sax::EntityResolver,
        ::jxx::org::xml::sax::DTDHandler,
        ::jxx::org::xml::sax::ContentHandler,
        ::jxx::org::xml::sax::ErrorHandler>;

    DefaultHandler();
    ~DefaultHandler() override;

    ::jxx::Ptr<::jxx::org::xml::sax::InputSource> resolveEntity(
        const ::jxx::Ptr<::jxx::lang::String>& publicId,
        const ::jxx::Ptr<::jxx::lang::String>& systemId) override;

    void notationDecl(
        const ::jxx::Ptr<::jxx::lang::String>& name,
        const ::jxx::Ptr<::jxx::lang::String>& publicId,
        const ::jxx::Ptr<::jxx::lang::String>& systemId) override;

    void unparsedEntityDecl(
        const ::jxx::Ptr<::jxx::lang::String>& name,
        const ::jxx::Ptr<::jxx::lang::String>& publicId,
        const ::jxx::Ptr<::jxx::lang::String>& systemId,
        const ::jxx::Ptr<::jxx::lang::String>& notationName) override;

    void setDocumentLocator(
        const ::jxx::Ptr<::jxx::org::xml::sax::Locator>& locator) override;
    void startDocument() override;
    void endDocument() override;
    void startPrefixMapping(
        const ::jxx::Ptr<::jxx::lang::String>& prefix,
        const ::jxx::Ptr<::jxx::lang::String>& uri) override;
    void endPrefixMapping(
        const ::jxx::Ptr<::jxx::lang::String>& prefix) override;
    void startElement(
        const ::jxx::Ptr<::jxx::lang::String>& uri,
        const ::jxx::Ptr<::jxx::lang::String>& localName,
        const ::jxx::Ptr<::jxx::lang::String>& qName,
        const ::jxx::Ptr<::jxx::org::xml::sax::Attributes>& attributes) override;
    void endElement(
        const ::jxx::Ptr<::jxx::lang::String>& uri,
        const ::jxx::Ptr<::jxx::lang::String>& localName,
        const ::jxx::Ptr<::jxx::lang::String>& qName) override;
    void characters(
        const ::jxx::lang::CharArray& characters,
        ::jxx::lang::jint start,
        ::jxx::lang::jint length) override;
    void ignorableWhitespace(
        const ::jxx::lang::CharArray& characters,
        ::jxx::lang::jint start,
        ::jxx::lang::jint length) override;
    void processingInstruction(
        const ::jxx::Ptr<::jxx::lang::String>& target,
        const ::jxx::Ptr<::jxx::lang::String>& data) override;
    void skippedEntity(
        const ::jxx::Ptr<::jxx::lang::String>& name) override;

    void warning(
        const ::jxx::Ptr<::jxx::org::xml::sax::SAXParseException>& exception) override;
    void error(
        const ::jxx::Ptr<::jxx::org::xml::sax::SAXParseException>& exception) override;
    void fatalError(
        const ::jxx::Ptr<::jxx::org::xml::sax::SAXParseException>& exception) override;
};

} // namespace jxx::org::xml::sax::helpers
