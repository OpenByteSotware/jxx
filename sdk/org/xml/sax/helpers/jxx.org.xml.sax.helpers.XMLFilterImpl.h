#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "org/xml/sax/jxx.org.xml.sax.ContentHandler.h"
#include "org/xml/sax/jxx.org.xml.sax.DTDHandler.h"
#include "org/xml/sax/jxx.org.xml.sax.EntityResolver.h"
#include "org/xml/sax/jxx.org.xml.sax.ErrorHandler.h"
#include "org/xml/sax/jxx.org.xml.sax.XMLFilter.h"

namespace jxx::org::xml::sax::helpers {

class XMLFilterImpl
    : public ::jxx::lang::ClassBase<
          XMLFilterImpl,
          ::jxx::lang::Object,
          ::jxx::org::xml::sax::XMLFilter,
          ::jxx::org::xml::sax::EntityResolver,
          ::jxx::org::xml::sax::DTDHandler,
          ::jxx::org::xml::sax::ContentHandler,
          ::jxx::org::xml::sax::ErrorHandler> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<
        XMLFilterImpl,
        JxxSuper,
        ::jxx::org::xml::sax::XMLFilter,
        ::jxx::org::xml::sax::EntityResolver,
        ::jxx::org::xml::sax::DTDHandler,
        ::jxx::org::xml::sax::ContentHandler,
        ::jxx::org::xml::sax::ErrorHandler>;

    XMLFilterImpl();
    explicit XMLFilterImpl(const ::jxx::Ptr<::jxx::org::xml::sax::XMLReader>& parent);
    ~XMLFilterImpl() override;

    void setParent(const ::jxx::Ptr<::jxx::org::xml::sax::XMLReader>& parent) override;
    ::jxx::Ptr<::jxx::org::xml::sax::XMLReader> getParent() const override;

    ::jxx::lang::jbool getFeature(const ::jxx::Ptr<::jxx::lang::String>& name) const override;
    void setFeature(const ::jxx::Ptr<::jxx::lang::String>& name, ::jxx::lang::jbool value) override;
    ::jxx::Ptr<::jxx::lang::Object> getProperty(const ::jxx::Ptr<::jxx::lang::String>& name) const override;
    void setProperty(const ::jxx::Ptr<::jxx::lang::String>& name, const ::jxx::Ptr<::jxx::lang::Object>& value) override;

    void setEntityResolver(const ::jxx::Ptr<::jxx::org::xml::sax::EntityResolver>& resolver) override;
    ::jxx::Ptr<::jxx::org::xml::sax::EntityResolver> getEntityResolver() const override;
    void setDTDHandler(const ::jxx::Ptr<::jxx::org::xml::sax::DTDHandler>& handler) override;
    ::jxx::Ptr<::jxx::org::xml::sax::DTDHandler> getDTDHandler() const override;
    void setContentHandler(const ::jxx::Ptr<::jxx::org::xml::sax::ContentHandler>& handler) override;
    ::jxx::Ptr<::jxx::org::xml::sax::ContentHandler> getContentHandler() const override;
    void setErrorHandler(const ::jxx::Ptr<::jxx::org::xml::sax::ErrorHandler>& handler) override;
    ::jxx::Ptr<::jxx::org::xml::sax::ErrorHandler> getErrorHandler() const override;

    void parse(const ::jxx::Ptr<::jxx::org::xml::sax::InputSource>& input) override;
    void parse(const ::jxx::Ptr<::jxx::lang::String>& systemId) override;

    ::jxx::Ptr<::jxx::org::xml::sax::InputSource> resolveEntity(
        const ::jxx::Ptr<::jxx::lang::String>& publicId,
        const ::jxx::Ptr<::jxx::lang::String>& systemId) override;

    void notationDecl(const ::jxx::Ptr<::jxx::lang::String>& name,
        const ::jxx::Ptr<::jxx::lang::String>& publicId,
        const ::jxx::Ptr<::jxx::lang::String>& systemId) override;
    void unparsedEntityDecl(const ::jxx::Ptr<::jxx::lang::String>& name,
        const ::jxx::Ptr<::jxx::lang::String>& publicId,
        const ::jxx::Ptr<::jxx::lang::String>& systemId,
        const ::jxx::Ptr<::jxx::lang::String>& notationName) override;

    void setDocumentLocator(const ::jxx::Ptr<::jxx::org::xml::sax::Locator>& locator) override;
    void startDocument() override;
    void endDocument() override;
    void startPrefixMapping(const ::jxx::Ptr<::jxx::lang::String>& prefix,
        const ::jxx::Ptr<::jxx::lang::String>& uri) override;
    void endPrefixMapping(const ::jxx::Ptr<::jxx::lang::String>& prefix) override;
    void startElement(const ::jxx::Ptr<::jxx::lang::String>& uri,
        const ::jxx::Ptr<::jxx::lang::String>& localName,
        const ::jxx::Ptr<::jxx::lang::String>& qName,
        const ::jxx::Ptr<::jxx::org::xml::sax::Attributes>& attributes) override;
    void endElement(const ::jxx::Ptr<::jxx::lang::String>& uri,
        const ::jxx::Ptr<::jxx::lang::String>& localName,
        const ::jxx::Ptr<::jxx::lang::String>& qName) override;
    void characters(const ::jxx::lang::CharArray& characters,
        ::jxx::lang::jint start, ::jxx::lang::jint length) override;
    void ignorableWhitespace(const ::jxx::lang::CharArray& characters,
        ::jxx::lang::jint start, ::jxx::lang::jint length) override;
    void processingInstruction(const ::jxx::Ptr<::jxx::lang::String>& target,
        const ::jxx::Ptr<::jxx::lang::String>& data) override;
    void skippedEntity(const ::jxx::Ptr<::jxx::lang::String>& name) override;

    void warning(const ::jxx::Ptr<::jxx::org::xml::sax::SAXParseException>& exception) override;
    void error(const ::jxx::Ptr<::jxx::org::xml::sax::SAXParseException>& exception) override;
    void fatalError(const ::jxx::Ptr<::jxx::org::xml::sax::SAXParseException>& exception) override;

private:
    void setupParse_();

    ::jxx::Ptr<::jxx::org::xml::sax::XMLReader> parent_;
    ::jxx::Ptr<::jxx::org::xml::sax::EntityResolver> entityResolver_;
    ::jxx::Ptr<::jxx::org::xml::sax::DTDHandler> dtdHandler_;
    ::jxx::Ptr<::jxx::org::xml::sax::ContentHandler> contentHandler_;
    ::jxx::Ptr<::jxx::org::xml::sax::ErrorHandler> errorHandler_;
};

} // namespace jxx::org::xml::sax::helpers
