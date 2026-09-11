#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "org/xml/sax/jxx.org.xml.sax.DTDHandler.h"
#include "org/xml/sax/jxx.org.xml.sax.DocumentHandler.h"
#include "org/xml/sax/jxx.org.xml.sax.EntityResolver.h"
#include "org/xml/sax/jxx.org.xml.sax.ErrorHandler.h"

namespace jxx::org::xml::sax {

class HandlerBase
    : public ::jxx::lang::ClassBase<
          HandlerBase,
          ::jxx::lang::Object,
          EntityResolver,
          DTDHandler,
          DocumentHandler,
          ErrorHandler> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<
        HandlerBase,
        JxxSuper,
        EntityResolver,
        DTDHandler,
        DocumentHandler,
        ErrorHandler>;

    HandlerBase();
    ~HandlerBase() override;

    ::jxx::Ptr<InputSource> resolveEntity(
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

    void setDocumentLocator(const ::jxx::Ptr<Locator>& locator) override;
    void startDocument() override;
    void endDocument() override;
    void startElement(
        const ::jxx::Ptr<::jxx::lang::String>& name,
        const ::jxx::Ptr<AttributeList>& attributes) override;
    void endElement(
        const ::jxx::Ptr<::jxx::lang::String>& name) override;
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

    void warning(const ::jxx::Ptr<SAXParseException>& exception) override;
    void error(const ::jxx::Ptr<SAXParseException>& exception) override;
    void fatalError(const ::jxx::Ptr<SAXParseException>& exception) override;
};

} // namespace jxx::org::xml::sax
